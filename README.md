# Sumo Knockoff — GAME 415 Class Project

**GAME 415 Advanced Principles: Unreal Networking** · SCAD Atlanta · Spring 2026
*Our Business is Fun.*

This is the shared class repo for the Sumo Knockoff networked game. We use it as the working base for in-class exercises, lobby/session work in Week 6, and as the reference codebase you can pull from at any point.

---

## Quick Start (Read This First)

You will only do these steps **once** per machine.

### 1. Install the prerequisites

- **Git** — [git-scm.com/downloads](https://git-scm.com/downloads)
- **Git LFS** — [git-lfs.com](https://git-lfs.com)  *(non-negotiable — this repo will not work without it)*
- **Unreal Engine 5.7** — you should already have this from the pre-quarter assignment

### 2. Initialize Git LFS on your machine (one-time)

Open a terminal anywhere and run:

```bash
git lfs install
```

You should see: `Git LFS initialized.`

### 3. Clone the repo

```bash
git clone https://github.com/<org>/<repo>.git
cd <repo>
```

The clone will pull text files first, then LFS objects (the `.uasset` and `.umap` files). On a slow connection this can take several minutes.

### 4. Open the project

Double-click `Sumo.uproject`. The project is bound to launcher UE 5.7, so it should open directly. First compile takes 10–30 minutes depending on your hardware.

---

## Working in the Repo

### Branch policy

- **`main`** — always playable, always compiles. Do not push directly here.
- **`feature/<your-name>-<short-description>`** — your working branch for any change. Push freely.
- Open a Pull Request to `main` when your feature is ready for review.

### Before you start working on a binary asset

If you're about to edit a `.uasset` or `.umap`, **lock it** so nobody else edits it concurrently:

```bash
git lfs lock Content/Maps/SumoArena.umap
```

When you're done and have pushed your changes, unlock:

```bash
git lfs unlock Content/Maps/SumoArena.umap
```

You can see who has what locked with `git lfs locks`.

### Daily workflow

```bash
git checkout main
git pull                           # get the latest
git checkout -b feature/yourname-thing
# ... do work in the editor ...
git add -A
git commit -m "Short description of change"
git push -u origin feature/yourname-thing
# Then open a Pull Request on GitHub
```

---

## Folder Structure

| Folder        | What's in it                                            | Tracked? |
| ------------- | ------------------------------------------------------- | -------- |
| `Source/`     | C++ source code                                         | Yes      |
| `Content/`    | Blueprints, maps, materials, art, audio                 | Yes (LFS)|
| `Config/`     | `.ini` configuration files                              | Yes      |
| `Plugins/`    | Custom plugins (engine plugins live in your UE install) | Yes      |
| `Binaries/`   | Compiled output                                         | **No**   |
| `Intermediate/` | Build intermediates                                   | **No**   |
| `Saved/`      | Local saves, logs, screenshots                          | **No**   |

The "No" folders regenerate locally — they're machine-specific and would create constant merge conflicts.

---

## Troubleshooting

**`fatal: Authentication failed`** when you push
→ You need a GitHub Personal Access Token, not your password. Generate one at github.com → Settings → Developer settings → Personal access tokens.

**Files showing as `pointer` text instead of binaries**
→ You skipped `git lfs install`. Run it, then `git lfs pull`.

**Everything is broken after a `git pull`**
→ Right-click `.uproject` → **Generate Visual Studio project files**. Then in the editor: **File → Refresh Visual Studio Project**.

---

## Help

- In-class: ask Professor Lindsey
- Async: post in the class Slack `#game415` channel
- For Git LFS specifically: [git-lfs.com/docs](https://git-lfs.com/docs)

---

*GAME 415 Advanced Principles: Unreal Networking · Professor Lindsey · SCAD Atlanta*
