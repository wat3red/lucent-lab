#pragma once

// Gaussian Blur FBO and Shaders Setup and Usage in OpenGL with ImGui overlay
namespace BlurEffect {
	const char* vertexShaderSource = R"(
        #version 330 core
        layout (location = 0) in vec2 aPos;
        out vec2 TexCoords;
        void main() {
            TexCoords = (aPos + 1.0) / 2.0;
            gl_Position = vec4(aPos, 0.0, 1.0);
        }
    )";

	const char* blurFragmentShaderSource = R"(
		#version 330 core
		out vec4 FragColor;
		in vec2 TexCoords;

		uniform sampler2D u_Texture;
		uniform vec2 u_ScreenSize;
		uniform bool u_Horizontal;
		uniform float u_BlurStrength; 

		const float weight[9] = float[] (
			0.0162162162, 0.0540540541, 0.1216216216, 0.1945945946, 0.2270270270, // центральный + 4 в сторону
			0.1945945946, 0.1216216216, 0.0540540541, 0.0162162162
		);

		void main() {
			vec2 texelSize = 1.0 / u_ScreenSize;
        
			const int blurRadius = 4;

			vec3 result = texture(u_Texture, TexCoords).rgb * weight[4]; // Центральный сэмпл

			if (u_Horizontal) {
				for (int i = 1; i <= blurRadius; ++i) {
					float offset = float(i) * u_BlurStrength;
					result += texture(u_Texture, TexCoords + vec2(texelSize.x * offset, 0.0)).rgb * weight[4 + i];
					result += texture(u_Texture, TexCoords - vec2(texelSize.x * offset, 0.0)).rgb * weight[4 - i];
				}
			} else { 
				for (int i = 1; i <= blurRadius; ++i) {
					float offset = float(i) * u_BlurStrength;
					result += texture(u_Texture, TexCoords + vec2(0.0, texelSize.y * offset)).rgb * weight[4 + i];
					result += texture(u_Texture, TexCoords - vec2(0.0, texelSize.y * offset)).rgb * weight[4 - i];
				}
			}
        
			FragColor = vec4(result, 1.0);
		}
	)";

	const char* displayFragmentShaderSource = R"(
		#version 330 core
		out vec4 FragColor;
		in vec2 TexCoords;
		uniform sampler2D u_Texture;
		void main() {
			FragColor = texture(u_Texture, TexCoords);
		}
	)";


	GLuint quadVAO = 0;
	GLuint fboScene, texScene;
	GLuint fboBlur, texBlur;
	GLuint shaderBlur, shaderDisplay;
	int screenWidth = 0, screenHeight = 0;
	bool isInitialized = false;

	GLuint CompileShader(GLenum type, const char* source) {
		GLuint shader = glCreateShader(type);
		glShaderSource(shader, 1, &source, NULL);
		glCompileShader(shader);
		GLint success;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if (!success) {
			char infoLog[512];
			glGetShaderInfoLog(shader, 512, NULL, infoLog);
			std::string errorMsg = oxorany("Shader compilation failed:\n") + std::string(infoLog);
			MessageBoxA(NULL, errorMsg.c_str(), oxorany("Shader Error"), MB_OK);
			return 0;
		}
		return shader;
	}

	GLuint CreateShaderProgram(const char* vsSource, const char* fsSource) {
		GLuint vertexShader = CompileShader(GL_VERTEX_SHADER, vsSource);
		GLuint fragmentShader = CompileShader(GL_FRAGMENT_SHADER, fsSource);
		if (vertexShader == 0 || fragmentShader == 0) return 0;

		GLuint program = glCreateProgram();
		glAttachShader(program, vertexShader);
		glAttachShader(program, fragmentShader);
		glLinkProgram(program);

		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);

		GLint success;
		glGetProgramiv(program, GL_LINK_STATUS, &success);
		if (!success) {
			char infoLog[512];
			glGetProgramInfoLog(program, 512, NULL, infoLog);
			std::string errorMsg = oxorany("Shader linking failed:\n") + std::string(infoLog);
			MessageBoxA(NULL, errorMsg.c_str(), oxorany("Shader Link Error"), MB_OK);
			return 0;
		}
		return program;
	}

	void CreateFBO(GLuint& fbo, GLuint& tex, int width, int height) {
		glGenFramebuffers(1, &fbo);
		glBindFramebuffer(GL_FRAMEBUFFER, fbo);

		glGenTextures(1, &tex);
		glBindTexture(GL_TEXTURE_2D, tex);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tex, 0);

		GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
		if (status != GL_FRAMEBUFFER_COMPLETE) {
			std::string reason;
			switch (status) {
			case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT: reason = "INCOMPLETE_ATTACHMENT"; break;
			case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT: reason = "MISSING_ATTACHMENT"; break;
			case GL_FRAMEBUFFER_UNSUPPORTED: reason = "UNSUPPORTED"; break;
			default: reason = "OTHER"; break;
			}
			MessageBoxA(NULL, (std::string("FBO error: ") + reason).c_str(), "FBO Error", MB_OK);
		}


		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void UpdateScreenSize() {
		GLint viewport[4];
		glGetIntegerv(GL_VIEWPORT, viewport);
		int newWidth = viewport[2];
		int newHeight = viewport[3];

		if (newWidth != screenWidth || newHeight != screenHeight) {
			screenWidth = newWidth;
			screenHeight = newHeight;

			// Удаляем старые ресурсы
			glDeleteFramebuffers(1, &fboScene);
			glDeleteTextures(1, &texScene);
			glDeleteFramebuffers(1, &fboBlur);
			glDeleteTextures(1, &texBlur);

			// Создаём новые с актуальными размерами
			CreateFBO(fboScene, texScene, screenWidth, screenHeight);
			CreateFBO(fboBlur, texBlur, screenWidth, screenHeight);
		}
	}

	void RenderFullscreenQuad() {
		if (quadVAO == 0) {
			float quadVertices[] = { -1.0f, 1.0f, -1.0f, -1.0f, 1.0f, 1.0f, 1.0f, -1.0f };
			GLuint quadVBO;
			glGenVertexArrays(1, &quadVAO);
			glGenBuffers(1, &quadVBO);
			glBindVertexArray(quadVAO);
			glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
		}
		glBindVertexArray(quadVAO);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	}

	void Initialize() {
		if (isInitialized) return;

		if (!gladLoadGL()) {
			MessageBoxA(NULL, oxorany("Failed to initialize GLAD"), oxorany("GLAD Error"), MB_OK);
			return;
		}

		/*std::cout << "================ OpenGL Info ================\n";
		std::cout << "Vendor:   " << glGetString(GL_VENDOR) << "\n";
		std::cout << "Renderer: " << glGetString(GL_RENDERER) << "\n";
		std::cout << "Version:  " << glGetString(GL_VERSION) << "\n";
		std::cout << "GLSL:     " << glGetString(GL_SHADING_LANGUAGE_VERSION) << "\n";

		GLint major, minor;
		glGetIntegerv(GL_MAJOR_VERSION, &major);
		glGetIntegerv(GL_MINOR_VERSION, &minor);
		std::cout << "Version (int): " << major << "." << minor << "\n";

		GLint maxTexSize;
		glGetIntegerv(GL_MAX_TEXTURE_SIZE, &maxTexSize);
		std::cout << "Max Texture Size: " << maxTexSize << "\n";

		GLint maxTexUnits;
		glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &maxTexUnits);
		std::cout << "Max Combined Texture Units: " << maxTexUnits << "\n";

		GLint maxVertexAttribs;
		glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &maxVertexAttribs);
		std::cout << "Max Vertex Attributes: " << maxVertexAttribs << "\n";

		GLint maxDrawBuffers;
		glGetIntegerv(GL_MAX_DRAW_BUFFERS, &maxDrawBuffers);
		std::cout << "Max Draw Buffers: " << maxDrawBuffers << "\n";

		GLint maxSamples;
		glGetIntegerv(GL_MAX_SAMPLES, &maxSamples);
		std::cout << "Max MSAA Samples: " << maxSamples << "\n";

		GLint maxColorAttachments;
		glGetIntegerv(GL_MAX_COLOR_ATTACHMENTS, &maxColorAttachments);
		std::cout << "Max FBO Color Attachments: " << maxColorAttachments << "\n";

		GLint numExtensions;
		glGetIntegerv(GL_NUM_EXTENSIONS, &numExtensions);
		std::cout << "Num Extensions: " << numExtensions << "\n";

		std::cout << "Extensions:\n";
		for (GLint i = 0; i < numExtensions; ++i) {
			std::cout << "  " << glGetStringi(GL_EXTENSIONS, i) << "\n";
		}

		std::cout << "=============================================\n";*/

		// 2. Получить реальные размеры окна
		GLint viewport[4];
		glGetIntegerv(GL_VIEWPORT, viewport);
		screenWidth = viewport[2];
		screenHeight = viewport[3];

		// 3. Создать FBO и скомпилировать шейдеры
		CreateFBO(fboScene, texScene, screenWidth, screenHeight);
		CreateFBO(fboBlur, texBlur, screenWidth, screenHeight); // Один FBO для обоих проходов

		shaderBlur = CreateShaderProgram(vertexShaderSource, blurFragmentShaderSource);
		shaderDisplay = CreateShaderProgram(vertexShaderSource, displayFragmentShaderSource);

		isInitialized = (shaderBlur != 0 && shaderDisplay != 0);
	}

	void Apply(float blurStrength) {
		if (!isInitialized) return;

		UpdateScreenSize();
		// --- СОХРАНЕНИЕ СОСТОЯНИЯ OPENGL ИГРЫ ---
		GLint last_active_texture; glGetIntegerv(GL_ACTIVE_TEXTURE, &last_active_texture);
		glActiveTexture(GL_TEXTURE0);
		GLint last_program; glGetIntegerv(GL_CURRENT_PROGRAM, &last_program);
		GLint last_texture; glGetIntegerv(GL_TEXTURE_BINDING_2D, &last_texture);
		GLint last_framebuffer; glGetIntegerv(GL_FRAMEBUFFER_BINDING, &last_framebuffer);
		GLint last_vertex_array; glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &last_vertex_array);
		GLboolean last_blend_enabled = glIsEnabled(GL_BLEND);
		GLint last_viewport[4]; glGetIntegerv(GL_VIEWPORT, last_viewport);

		// --- НАЧАЛО РЕНДЕРА РАЗМЫТИЯ ---

		// 1. Копируем содержимое экрана (back buffer) в нашу текстуру
		glBindTexture(GL_TEXTURE_2D, texScene);
		glBindFramebuffer(GL_READ_FRAMEBUFFER, 0); // Читаем из стандартного буфера
		glCopyTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 0, 0, screenWidth, screenHeight);

		// 2. Проход горизонтального размытия
		glBindFramebuffer(GL_FRAMEBUFFER, fboBlur);
		glUseProgram(shaderBlur);
		glUniform1i(glGetUniformLocation(shaderBlur, oxorany("u_Texture")), 0);
		glUniform1f(glGetUniformLocation(shaderBlur, oxorany("u_BlurStrength")), blurStrength);
		glUniform2f(glGetUniformLocation(shaderBlur, oxorany("u_ScreenSize")), screenWidth, screenHeight);
		glUniform1i(glGetUniformLocation(shaderBlur, oxorany("u_Horizontal")), true);
		glBindTexture(GL_TEXTURE_2D, texScene);
		RenderFullscreenQuad();

		// 3. Проход вертикального размытия
		glBindFramebuffer(GL_FRAMEBUFFER, fboScene); // Пишем результат в texScene
		glUniform1i(glGetUniformLocation(shaderBlur, oxorany("u_Horizontal")), false);
		glBindTexture(GL_TEXTURE_2D, texBlur); // Читаем из результата горизонтального размытия
		RenderFullscreenQuad();

		// 4. Отрисовка финального размытого изображения на экран
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glUseProgram(shaderDisplay);
		glUniform1i(glGetUniformLocation(shaderDisplay, oxorany("u_Texture")), 0);
		glBindTexture(GL_TEXTURE_2D, texScene);
		RenderFullscreenQuad();

		// --- ВОССТАНОВЛЕНИЕ СОСТОЯНИЯ OPENGL ИГРЫ ---
		glViewport(last_viewport[0], last_viewport[1], (GLsizei)last_viewport[2], (GLsizei)last_viewport[3]);
		glBindVertexArray(last_vertex_array);
		glBindFramebuffer(GL_FRAMEBUFFER, last_framebuffer);
		glBindTexture(GL_TEXTURE_2D, last_texture);
		if (last_blend_enabled) glEnable(GL_BLEND); else glDisable(GL_BLEND);
		glUseProgram(last_program);
		glActiveTexture(last_active_texture);
	}
}