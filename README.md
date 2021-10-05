# savingFileByUTF16
C program that transform a UTF-16 txt file to another OS.<br />
The program supports 3 options of OS: Mac, Unix, Windows and also:
* Copy an existing UTF-16 file
* Transform from\to Big-Endian to\from Little-Endian<br />

## How to use
After using `make` you can use the following command-line arguments:
> source-file-name | new-file-name | source-file-os-flag | new-file-os-flag | byte-order-flag <br />

Name  | Meaning
------------- | -------------
source-file-name  | .txt valid UTF-16 source file
new-file-name	  | .txt file to be overriden with output data
source-file-os-flag	  | source file OS
new-file-os-flag  | output file OS
byte-order-flag	  | indicator to switch Endians

* Source & output file-os-flag : `-mac`, `-unix`, `-windows`
* byte-order-flag : `-swap`, `-keep`

