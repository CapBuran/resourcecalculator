@ECHO OFF

CALL config64.cmd

SET OldDir=%CD%

"%MyProjectDir%\Program\%Config%\ExtractJPGFromPCAP.exe" -input "%MyRepository%\Data\test.pcap" -output "test.jpg"



::PAUSE