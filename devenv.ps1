function Invoke-CmdScript {
  param(
    [String] $scriptName
  )

  $cmdLine = """$scriptName"" $args & set"
  & $Env:SystemRoot\system32\cmd.exe /c $cmdLine |
  select-string '^([^=]*)=(.*)$' | foreach-object {
    $varName = $_.Matches[0].Groups[1].Value
    $varValue = $_.Matches[0].Groups[2].Value
    set-item Env:$varName $varValue
  }
}

if([string]::IsNullOrEmpty($args)){
  Write-Output "specify environment"
  exit
}

Write-Output "setting powershell environment"
Write-Output "invoking vcvarall.bat with args: $args"
Invoke-CmdScript "C:\Program Files (x86)\Microsoft Visual Studio 14.0\VC\vcvarsall.bat" $args
Write-Output "done"
