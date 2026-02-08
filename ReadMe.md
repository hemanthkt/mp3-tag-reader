# MP3 Tag Reader and Editor (ID3v2)

This is a refactored version of the MP3 tag editor with separate modules for better code organization.

## File Structure

```
main.c      - Main program entry point and command-line argument handling
view.c      - View/display MP3 tag information
view.h      - View module header
edit.c      - Edit MP3 tag information
edit.h      - Edit module header
types.h     - Common type definitions (OperationType enum)
```

## Key Improvements

1. **Modular Design**: Separated view and edit functionality into their own files
2. **Better Encapsulation**: Helper functions like `printDetails` are now static within their modules
3. **Error Handling**: Added file open checks and better error messages
4. **Include Guards**: Proper include guards in all header files
5. **Resource Management**: Added fclose() calls to prevent resource leaks

## Building

```bash
make
```

This will create an executable called `mp3tag`.

To clean build artifacts:

```bash
make clean
```

## Usage

### View MP3 Tags

```bash
./mp3tag -v <mp3file>
```

Example:

```bash
./mp3tag -v song.mp3
```

### Edit MP3 Tags

```bash
./mp3tag -e <tag> <new_value> <mp3file>
```

Available tags:

- `-t` : Title (TIT2)
- `-a` : Artist (TPE1)
- `-A` : Album (TALB)
- `-y` : Year (TYER)
- `-m` : Music/Genre (TCON)
- `-c` : Composer (COMM)

Example:

```bash
./mp3tag -e -t "New Song Title" song.mp3
```

The edited file will be saved as `test.mp3`.

## Supported Tags

The program supports the following ID3v2 tags:

- **TPE1**: Artist
- **TIT2**: Title
- **TALB**: Album
- **TYER**: Year
- **TCON**: Music/Genre
- **COMM**: Composer

## Technical Details

- Reads ID3v2 tag format
- Skips the 10-byte ID3v2 header
- Reads up to 6 tag frames
- Each frame has a 4-byte tag ID, 4-byte size, and 2-byte flags
- Supports basic tag editing with proper size recalculation
