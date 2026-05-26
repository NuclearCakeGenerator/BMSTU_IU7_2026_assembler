To install dependencies on linux:

1. Install `llvm-vs-code-extensions.lldb-dap` extension in VS Code.
2. run `setup.sh` via root

or

2. Install `make`, `nasm` etc manually:

```bash
sudo apt update && sudo apt upgrade -y && apt install -y nasm make gcc gdb libgtk-3-dev g++-aarch64-linux-gnu
```

.vscode configuration will allow you to build and run your specific lab in debug mode by simply press `F5` when any file of this lab is currently opened in editor.
