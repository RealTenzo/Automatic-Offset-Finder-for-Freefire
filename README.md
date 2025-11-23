# Automatic Offset Finder for Free Fire

A tool that scans your **dump.cs** file and automatically finds the latest **offsets** and **bone offsets** for Free Fire. No manual searching needed — drop your dump and get clean output files quickly.

---

## Features

* Automatic detection of classes and fields
* Extracts offsets and bone offsets
* Generates `offsets.txt` and `bones.txt`
* Fast scanning for large dump files
* Works with updated dump files
* Simple console interface
* Made by Tenzo

---

## How to Use

1. Run the tool
2. Enter the path to your `dump.cs` file
3. Wait for the extraction process
4. Output files created in the same folder:

   * `offsets.txt`
   * `bones.txt`

---

## Output Example

```csharp
namespace DarknetHaxor
{
    internal static class Offsets
    {
        // General
        internal static uint CurrentMatch = 0x1234;
        internal static uint LocalPlayer = 0x5678;
        
        // Player
        internal static uint Player_Name = 0x9ABC;
        internal static uint Player_IsDead = 0xDEF0;
    }
}
```
