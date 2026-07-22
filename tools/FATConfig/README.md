# FATConfig keybind patch

`Build-FATConfig.ps1` preserves the original configurator and adds support for
the client binding identifiers `Deploy` and `ClassAbility`. It also adds the
matching `Y` and `K` defaults used by the client. The key list additionally
supports `Mouse Button 4` and `Mouse Button 5`.

Build from an existing FATConfig executable:

```powershell
.\Build-FATConfig.ps1 -InputExe .\FATConfig.exe -OutputExe .\FATConfig.patched.exe
```
