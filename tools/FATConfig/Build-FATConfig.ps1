param(
    [Parameter(Mandatory = $true)]
    [string] $InputExe,

    [Parameter(Mandatory = $true)]
    [string] $OutputExe
)

$ErrorActionPreference = 'Stop'

$ildasmCandidates = @(
    'C:\Program Files (x86)\Microsoft SDKs\Windows\v7.0A\Bin\ildasm.exe',
    'C:\Program Files (x86)\Microsoft SDKs\Windows\v7.0A\Bin\NETFX 4.0 Tools\ildasm.exe'
)
$ilasmCandidates = @(
    "$env:WINDIR\Microsoft.NET\Framework\v4.0.30319\ilasm.exe",
    "$env:WINDIR\Microsoft.NET\Framework64\v4.0.30319\ilasm.exe"
)

$ildasm = $ildasmCandidates | Where-Object { Test-Path -LiteralPath $_ } | Select-Object -First 1
$ilasm = $ilasmCandidates | Where-Object { Test-Path -LiteralPath $_ } | Select-Object -First 1
if (-not $ildasm -or -not $ilasm) {
    throw 'ILDasm/ILAsm from .NET Framework SDK is required.'
}

$inputPath = (Resolve-Path -LiteralPath $InputExe).Path
$outputPath = [IO.Path]::GetFullPath($OutputExe)
$outputDirectory = Split-Path -Parent $outputPath
if (-not (Test-Path -LiteralPath $outputDirectory)) {
    New-Item -ItemType Directory -Path $outputDirectory | Out-Null
}

$workDirectory = Join-Path ([IO.Path]::GetTempPath()) ("fatconfig-build-" + [Guid]::NewGuid().ToString('N'))
New-Item -ItemType Directory -Path $workDirectory | Out-Null

try {
    $ilPath = Join-Path $workDirectory 'FATConfig.il'
    $resourcePath = Join-Path $workDirectory 'FATConfig.res'

    Push-Location $workDirectory
    try {
        & $ildasm $inputPath /nobar "/out=$ilPath"
        if ($LASTEXITCODE -ne 0) {
            throw "ILDasm failed with exit code $LASTEXITCODE."
        }
    }
    finally {
        Pop-Location
    }

    $il = [IO.File]::ReadAllText($ilPath).Replace("`r`n", "`n")
    if ($il -notmatch 'FATConfig\.KeybindForm') {
        throw 'Input file is not a supported FATConfig.exe assembly.'
    }

    if ($il -notmatch 'ldstr\s+"ClassAbility"') {
        $loadIndexAnchor = @'
    IL_014b:  ldstr      "StopMove"
    IL_0150:  ldstr      "Stop moving!"
    IL_0155:  call       void FATConfig.KeybindForm::Index(string,
                                                           string)
    IL_015a:  nop
    IL_015b:  ret
'@
        $loadIndexReplacement = @'
    IL_014b:  ldstr      "StopMove"
    IL_0150:  ldstr      "Stop moving!"
    IL_0155:  call       void FATConfig.KeybindForm::Index(string,
                                                           string)
    IL_015a:  nop
    ldstr      "Deploy"
    ldstr      "Deploy"
    call       void FATConfig.KeybindForm::Index(string,
                                                  string)
    nop
    ldstr      "ClassAbility"
    ldstr      "Class ability"
    call       void FATConfig.KeybindForm::Index(string,
                                                  string)
    nop
    IL_015b:  ret
'@
        if (-not $il.Contains($loadIndexAnchor)) {
            throw 'Could not locate KeybindForm.LoadIndex patch point.'
        }
        $il = $il.Replace($loadIndexAnchor, $loadIndexReplacement)

        $getNameAnchor = @'
    IL_03b6:  nop
    IL_03b7:  ldloc.0
    IL_03b8:  ldstr      "Use Items - "
'@
        $getNameReplacement = @'
    IL_03b6:  nop
    ldarg.0
    ldfld      string FATConfig.KeyBind::Type
    ldstr      "Deploy"
    callvirt   instance bool [mscorlib]System.String::Equals(string)
    brfalse.s  FATCONFIG_CHECK_CLASS_ABILITY
    ldloc.0
    ldstr      "Deploy"
    call       string [mscorlib]System.String::Concat(string,
                                                       string)
    stloc.0
    br         IL_041c

  FATCONFIG_CHECK_CLASS_ABILITY:
    ldarg.0
    ldfld      string FATConfig.KeyBind::Type
    ldstr      "ClassAbility"
    callvirt   instance bool [mscorlib]System.String::Equals(string)
    brfalse.s  FATCONFIG_UNKNOWN_BIND
    ldloc.0
    ldstr      "Class ability"
    call       string [mscorlib]System.String::Concat(string,
                                                       string)
    stloc.0
    br         IL_041c

  FATCONFIG_UNKNOWN_BIND:
    IL_03b7:  ldloc.0
    IL_03b8:  ldstr      "Use Items - "
'@
        if (-not $il.Contains($getNameAnchor)) {
            throw 'Could not locate KeyBind.GetName patch point.'
        }
        $il = $il.Replace('IL_03b4:  br.s       IL_041c', 'IL_03b4:  br         IL_041c')
        $il = $il.Replace($getNameAnchor, $getNameReplacement)

        $defaultsAnchor = @'
    IL_02d1:  call       instance void FATConfig.IniParser::AddNewSetting(string,
                                                                          string,
                                                                          string)
    IL_02d6:  nop
    IL_02d7:  ret
  } // end of method IniParser::MakeDefaultConfig
'@
        $defaultsReplacement = @'
    IL_02d1:  call       instance void FATConfig.IniParser::AddNewSetting(string,
                                                                          string,
                                                                          string)
    IL_02d6:  nop
    ldarg.0
    ldstr      "Bindings"
    ldstr      "Deploy"
    ldstr      "Y"
    call       instance void FATConfig.IniParser::AddNewSetting(string,
                                                                 string,
                                                                 string)
    nop
    ldarg.0
    ldstr      "Bindings"
    ldstr      "ClassAbility"
    ldstr      "K"
    call       instance void FATConfig.IniParser::AddNewSetting(string,
                                                                 string,
                                                                 string)
    nop
    IL_02d7:  ret
  } // end of method IniParser::MakeDefaultConfig
'@
        if (-not $il.Contains($defaultsAnchor)) {
            throw 'Could not locate IniParser.MakeDefaultConfig patch point.'
        }
        $il = $il.Replace($defaultsAnchor, $defaultsReplacement)
    }

    if ($il -notmatch 'ldstr\s+"Mouse Button 4"') {
        $mouseArraySizeAnchor = @'
    IL_04f4:  ldc.i4.s   72
    IL_04f6:  newarr     [mscorlib]System.Object
'@
        $mouseArraySizeReplacement = @'
    IL_04f4:  ldc.i4.s   74
    IL_04f6:  newarr     [mscorlib]System.Object
'@
        if (-not $il.Contains($mouseArraySizeAnchor)) {
            throw 'Could not locate the key list size patch point.'
        }
        $il = $il.Replace($mouseArraySizeAnchor, $mouseArraySizeReplacement)

        $mouseItemsAnchor = @'
    IL_0772:  ldloc.1
    IL_0773:  ldc.i4.s   71
    IL_0775:  ldstr      "Right"
    IL_077a:  stelem.ref
    IL_077b:  ldloc.1
    IL_077c:  callvirt   instance void [System.Windows.Forms]System.Windows.Forms.ComboBox/ObjectCollection::AddRange(object[])
'@
        $mouseItemsReplacement = @'
    IL_0772:  ldloc.1
    IL_0773:  ldc.i4.s   71
    IL_0775:  ldstr      "Right"
    IL_077a:  stelem.ref
    ldloc.1
    ldc.i4.s   72
    ldstr      "Mouse Button 4"
    stelem.ref
    ldloc.1
    ldc.i4.s   73
    ldstr      "Mouse Button 5"
    stelem.ref
    IL_077b:  ldloc.1
    IL_077c:  callvirt   instance void [System.Windows.Forms]System.Windows.Forms.ComboBox/ObjectCollection::AddRange(object[])
'@
        if (-not $il.Contains($mouseItemsAnchor)) {
            throw 'Could not locate the key list items patch point.'
        }
        $il = $il.Replace($mouseItemsAnchor, $mouseItemsReplacement)
    }

    [IO.File]::WriteAllText($ilPath, $il, [Text.UTF8Encoding]::new($false))

    Push-Location $workDirectory
    try {
        & $ilasm $ilPath /exe /quiet "/output=$outputPath" "/resource=$resourcePath"
        if ($LASTEXITCODE -ne 0) {
            throw "ILAsm failed with exit code $LASTEXITCODE."
        }
    }
    finally {
        Pop-Location
    }
}
finally {
    Remove-Item -LiteralPath $workDirectory -Recurse -Force
}
