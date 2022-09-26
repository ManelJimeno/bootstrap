#!/usr/bin/pwsh
<#
.SYNOPSIS
    Load Visual Studio environment
#>

function VersionToVersionNumber
{
    param (
        [parameter(HelpMessage = "VS version")]
        [string]$vsVersion
    )

    if ( $yearToVersion.Contains($vsVersion))
    {
        return $yearToVersion[$vsVersion]
    }

    $results = $yearToVersion.Keys.Where({ $yearToVersion[$_] -eq "True" })
    if ($results.Count -gt 0)
    {
        return $results
    }

    return $vsVersion
}

function findWithVswhere
{
    param([string]$pattern, [string]$versionPattern)
    try
    {
        $installationPath = $( Invoke-Expression "vswhere -products * $versionPattern -prerelease -property installationPath" )
        return $installationPath + '\' + $pattern
    }
    catch
    {
        Write-Error "vswhere failed: $_"
    }
    return null
}

function findVcVarsall
{
    param (
        [parameter(HelpMessage = "VS version")]
        [string]$vsVersion
    )

    if ( [string]::IsNullOrWhitespace($vsVersion))
    {
        $versionPattern = "-latest"
    }
    else
    {
        $versionNumber = VersionToVersionNumber($vsVersion)
        $upperBound = $versionNumber.split('.')[0] + '.9'
        $versionPattern = `-version "$versionNumber,$upperBound"`

    }

    $path = findWithVswhere 'VC\Auxiliary\Build\vcvarsall.bat'  "$versionPattern"
    if (Test-Path -Path $path)
    {
        Write-Host "Found with vswhere: $path" -ForegroundColor Green
        return $path
    }


    Write-Host "Not found with vswhere" -ForegroundColor Yellow

    throw "Microsoft Visual Studio not found"
}

function Set-VsCmd-VSWhere
{
    param(
        [parameter(HelpMessage = "Enter VS version as 2017, 2019 or 2022")]
        [ValidateSet(2017, 2019, 2022)]
        [string]$version,
        [parameter(HelpMessage = "Architedture version")]
        [ValidateSet("win32", "win64", "x86_64", "x86-64")]
        [string]$arch = 'win64',
        [parameter(HelpMessage = "SDK version")]
        [string]$sdk,
        [parameter(HelpMessage = "Universal Windows Platform")]
        [switch]$uwp,
        [parameter(HelpMessage = "VS toolset")]
        [int]$toolset,
        [parameter(HelpMessage = "Spectre")]
        [switch]$spectre
    )

    if (-not$IsWindows)
    {
        Write-Host "It's only for windows" -ForegroundColor Red
    }

    $programFilesX86 = ${env:ProgramFiles(x86)}
    $programFiles = $env:ProgramFiles
    $editions = ('Enterprise', 'Professional', 'Community')
    $years = ('2022', '2019', '2017')
    $yearToVersion = @{
        '2022' = '17.0';
        '2019' = '16.0';
        '2017' = '15.0';
        '2015' = '14.0';
        '2013' = '12.0'
    }
    $archAliases = @{
        'win32' = 'x86';
        'win64' = 'x64';
        'x86_64' = 'x64';
        'x86-64' = 'x64'
    }
    $vsPath = "$programFilesX86\Microsoft Visual Studio\Installer"

    # Add standard location of "vsPath" to PATH, in case it's not there.
    $env:PATH += ";$vsPath"

    # Translate architecture to commont definition
    $arguments = @()
    $arguments += $archAliases[$arch.ToLower()]


    # Compose arguments
    if ($uwp)
    {
        $arguments += 'uwp'
    }
    if ($sdk)
    {
        $arguments += $sdk
    }
    if ($toolset)
    {
        $arguments += "-vcvars_ver=$toolset"
    }
    if ($spectre)
    {
        $arguments += '-vcvars_spectre_libs=spectre'
    }

    $vcvars = "`"$( findVcvarsall($vsVersion) )`" $( $arguments -join " " ) "

    $cmdOutputString = $( cmd /c "set && echo `"\f`" && $( $vcvars ) && echo `"\f`" && set" )
    $cmdOutputParts = @(@(), @(), @())
    $position = 0
    foreach ($line in $cmdOutputString)
    {
        if ($line -eq '"\f" ')
        {
            $position += 1
        }
        else
        {
            $cmdOutputParts[$position] += $line
        }
    }

    $oldEnvironment = $cmdOutputParts[0]
    $vcvarsOutput = $cmdOutputParts[1]
    $newEnvironment = $cmdOutputParts[2]

    $errorMessages = $yearToVersion.Keys.Where({
        $vcvarsOutput[$_] -match "/^\[ERROR.*\]/"
    })

    if ($errorMessages.Count -gt 0)
    {
        $erroMessage = "Invalid parameters `n " + (errorMessages -join "`n")
        throw $erroMessage
    }

    $oldEnvVars = @{ }
    foreach ($line in $oldEnvironment)
    {
        if ($line -match "(.*?)=(.*)")
        {
            $oldEnvVars[$matches[1]] = $matches[2]
        }
    }

    $newEnvVars = @{ }
    foreach ($line in $newEnvironment)
    {
        if (-not $line.Contains('='))
        {
            continue;
        }

        if ($line -match "(.*?)=(.*)")
        {
            $newEnvVars[$matches[1]] = $matches[2]
            if ($newEnvVars[$matches[1]] -ne $oldEnvVars[$matches[1]])
            {
                Write-Host "Setting $( $matches[1] )" -ForegroundColor Yellow
                Set-Item -force -path "ENV:\$( $matches[1] )" -value "$( $matches[2] )"
            }
        }
    }

    Write-Host "`nVisual Studio $version Command Prompt variables set." -ForegroundColor Green
}

function Set-VsCmd
{
    param(
        [parameter(Mandatory, HelpMessage = "Enter VS version as 2019 or 2022")]
        [ValidateSet(2019, 2022)]
        [int]$version
    )

    $paths = ( "C:\Program Files (x86)\Microsoft Visual Studio\$version\BuildTools\VC\Auxiliary\Build",
    "C:\Program Files (x86)\Microsoft Visual Studio\$version\Community\VC\Auxiliary\Build",
    "C:\Program Files (x86)\Microsoft Visual Studio\$version\Professional\VC\Auxiliary\Build",
    "C:\Program Files (x86)\Microsoft Visual Studio\$version\Enterprise\VC\Auxiliary\Build",
    "D:\Program Files (x86)\Microsoft Visual Studio\$version\BuildTools\VC\Auxiliary\Build",
    "D:\Program Files (x86)\Microsoft Visual Studio\$version\Community\VC\Auxiliary\Build",
    "D:\Program Files (x86)\Microsoft Visual Studio\$version\Professional\VC\Auxiliary\Build",
    "D:\Program Files (x86)\Microsoft Visual Studio\$version\Enterprise\VC\Auxiliary\Build")
    $targetDir = ""

    foreach ($path in $paths)
    {
        if ((Test-Path (Join-Path $path "vcvars64.bat")))
        {
            $targetDir = $path
            break;
        }
    }

    if ( [string]::IsNullOrWhitespace($targetDir))
    {
        Write-Host "Error: Visual Studio $version not installed" -ForegroundColor Red
        exit -1
    }

    Push-Location $targetDir
    cmd /c "vcvars64.bat&set" |
            ForEach-Object {
                if ($_ -match "(.*?)=(.*)")
                {
                    Set-Item -force -path "ENV:\$( $matches[1] )" -value "$( $matches[2] )"
                }
            }
    Pop-Location
    Write-Host "`nVisual Studio $version Command Prompt variables set." -ForegroundColor Green
}
