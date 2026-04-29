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

## Source Control Setup

### In the Unreal Editor

1. **Edit → Editor Preferences → Source Control**
2. Set **Provider** to **Git (beta version)**
3. Point **Git Binary Path** to your `git.exe` (e.g. `C:\Program Files\Git\bin\git.exe`)
4. Click **Accept Settings** — the bottom-left of the editor should now show a green source control icon

### In Visual Studio

1. Open the `.sln` (generate it from the `.uproject` right-click menu if needed)
2. **Git → Settings → Source Control → Current Source Control Plug-in** should say **Git**
3. **View → Git Changes** to see your working tree, stage, and commit from within VS
4. Use **Git → Manage Branches** to create feature branches and switch between them

> **Note:** Do NOT build from VS with the editor open — it causes `SetEnv` MSBuild errors. Close the editor first, or build from the terminal with the UBT batch file.

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

## Demo Code: SumoPickup

`Source/Sumo/Public/SumoPickup.h` · `Source/Sumo/Private/SumoPickup.cpp`

A simple single-player pickup actor used as the first in-class C++ example. It does **not** replicate — this is just the basics. Covers:

- **Components** — `UStaticMeshComponent` (visible mesh) + `USphereComponent` (overlap trigger)
- **UPROPERTY** — editor-tweakable values: `RotationSpeed`, `BobAmplitude`, `BobSpeed`, `PickupSound`
- **Tick** — spins and bobs the pickup using a sine wave
- **Overlap events** — `OnComponentBeginOverlap` delegate binding, pawn filtering
- **BlueprintImplementableEvent** — `OnPickedUp()` lets BP subclasses add effects, scoring, etc.

Students: create your own pickup by making a **Blueprint subclass** of `SumoPickup`, assigning a mesh and sound, and implementing the `OnPickedUp` event in the event graph.

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
