# Publish checklist (GitHub)

Local public export is ready at `D:\Desktop\standoff2_public` (fresh `main`, one commit).
Private monorepo with full history stays at `D:\Desktop\standoff2_internal` — **do not push that**.

## Gate results (this machine)

| Gate | Result |
|---|---|
| Export secret scan | PASS — only `REPLACE_WITH_ROTATED_*` placeholders |
| Binaries in export | PASS — none |
| `auth_secrets.h` in export | PASS — not present (example only) |
| Offline auth default | PASS — `ENABLE_REMOTE_AUTH` off |
| Third-party licenses | PASS — colocated + `licenses/` |
| Payload NDK build | PASS — `payload/libs/armeabi-v7a/libdaemon.so` (private tree) |
| Client MSVC build | PASS — `client/x64/Release/LucentClient.dll` (private tree) |
| Fresh history | PASS — export `git log` has 1 commit |

## Manual steps you still own

1. **Rotate the old master key** on any live auth backend (treat as public forever).
2. **Create GitHub repo** (private first), e.g. `wat3red/standoff2_internal` after renaming/archiving the old one, or `wat3red/standoff2-lab`.
   ```bat
   :: from export
   git remote add origin https://github.com/wat3red/<repo>.git
   git push -u origin main
   ```
3. **Browse the public files on github.com** before flipping visibility.
4. **Archive** `https://github.com/wat3red/standoff2_internal_client` with a README note pointing at the monorepo.
5. Keep `D:\Desktop\standoff2_internal` **private** as the working/history repo.

## Optional

- Pin `docs/PORTFOLIO.md` in the GitHub about/description.
- Write a short architecture post linking the public repo.
