Write-Host "[PUBLISH] WIN32"
New-Item -ItemType "directory" -Path "./dist" -Force
Compress-Archive -Path "./build/Release/bin/*" -DestinationPath "./dist/NZJqGUI2-v0.1.0.zip" -Force
