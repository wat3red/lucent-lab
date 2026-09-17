// Copy to auth_secrets.h and fill in real values.
// auth_secrets.h is gitignored — never commit live keys.
#pragma once

#define VERIFY_LINK "https://example.invalid/verify.php"
#define MASTER_KEY "REPLACE_WITH_ROTATED_KEY"
#define PBKDF2_SALT "REPLACE_WITH_ROTATED_SALT"
