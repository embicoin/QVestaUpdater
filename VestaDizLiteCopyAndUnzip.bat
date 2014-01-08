@echo off
rmdir /S /Q VestaDizLite
mkdir VestaDizLite
cd VestaDizLite
copy "C:\Users\1\Dropbox\VestaLite (2)\VestaDizLite.zip" VestaDizLite.zip
"C:\Program Files\HaoZip\HaoZipC.exe" e VestaDizLite.zip
start /wait VestaDizLite.exe /verysilent
