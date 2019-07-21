# Minimal implementation of stealthy export table view
*Gently given* access to export tables of loaded modules in 32 bit executables.

Features:
* No usage of winapi functions like GetModuleHandle(), GetProcAddress() or LdrGetProcedureAddress()
* Handes import by name and ordinal
* Uses compile time hashing, no text strings exposed in the final binary!
* Simplest api in the universe
* Reading unit tests is enough to get the grip