# interleave
Utility for combining the contents of multiple files, with bytes from each source file interwoven in the output data.

As an example, consider two files with 8 bytes of data:

File A: `00 01 02 03 04 05 06 07`

File B: `08 09 0A 0B 0C 0D 0E 0F`

If we interleave these files with the default byte width of 2, the output file will contain the following:

`00 01 08 09 02 03 0A 0B 04 05 0C 0D 06 07 0E 0F`

One of the most common use cases for this is combining dumps of data stored on multiple ROM chips with data split on even and odd bytes.

## Usage
Switches come after the executable, followed by the list of files to be interleaved. Files are processed in the order they are listed. Up to 256 files can be combined, with a minimum of two necessary.

Example:
```interleave --output=output_file --width=2 source1 source2```

## Switches
```--output```, ```-o```

Optional. Specifes the output file to which combined data will be written. If not specified, will use stdout.

```--width```, ```-w```

Optional. Specified the width (in bytes) to interleave. Default is 2.

```--skip-after```,```a```

Optional. Specifes a number of bytes to ignore after the current read. Default is 0.

```--skip-before```,```b```

Options. Specified a number of bytes to ignroe before the next read. Deafult is 0.
