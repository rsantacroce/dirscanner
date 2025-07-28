# Directory Size Scanner

A fast and beautiful C program that scans directories on macOS and reports their sizes with colorful terminal output and optional HTML report generation.

![Directory Scanner Demo](https://img.shields.io/badge/platform-macOS-blue) ![License](https://img.shields.io/badge/license-MIT-green)

## Features

🎨 **Colorful Terminal Output**
- Color-coded directory sizes (Red for >10GB, Magenta for >1GB, etc.)
- Unicode progress bars
- Emoji indicators for better visual appeal
- Real-time scanning progress

📊 **HTML Report Generation**
- Beautiful, responsive HTML reports
- Interactive visual bars showing relative sizes
- Professional styling with hover effects
- Percentage breakdown and summary statistics

⚡ **Performance**
- Fast recursive directory scanning
- Efficient size calculation
- Handles large directory structures
- Graceful permission error handling

## Installation

### Prerequisites
- macOS (tested on macOS 10.12+)
- GCC compiler (included with Xcode Command Line Tools)

### Install Xcode Command Line Tools (if not already installed)
```bash
xcode-select --install
```

### Compile the Program
```bash
git clone <repository-url>
cd directory-size-scanner
gcc -std=c99 -o dirscanner dirscanner.c
```

## Usage

### Basic Usage
```bash
# Scan your home directory (default)
./dirscanner

# Scan a specific directory
./dirscanner /Applications

# Scan current directory
./dirscanner .
```

### Generate HTML Report
```bash
# Generate HTML report for home directory
./dirscanner --html

# Generate HTML report for specific directory
./dirscanner ~/Downloads --html

# You can also use the short flag
./dirscanner ~/Desktop -h
```

### Example Output

**Terminal Output:**
```
📁 Directory Size Scanner
=================================

🔍 Scanning /Users/username...
  📂 Calculating size of Documents... 2.34 GB
  📂 Calculating size of Downloads... 856.23 MB
  📂 Calculating size of Pictures... 12.45 GB

📊 Directory sizes (sorted by size):
=====================================

  1. Pictures              12.45 GB ████████████████████ 100.0%
  2. Documents              2.34 GB ███░░░░░░░░░░░░░░░░░  18.8%
  3. Downloads            856.23 MB ██░░░░░░░░░░░░░░░░░░   6.9%

📈 Summary:
Total size of 3 directories: 15.63 GB
Scanned path: /Users/username
```

**HTML Report:**
The HTML report provides a professional, web-based view with:
- Interactive progress bars
- Color-coded size categories
- Detailed statistics
- Responsive design that works on all devices

## Command Line Options

| Option | Description |
|--------|-------------|
| `--html` or `-h` | Generate HTML report (`directory_report.html`) |
| `<path>` | Specify directory to scan (default: home directory) |

## Size Categories

The program uses color coding to quickly identify large directories:

| Color | Size Range | Description |
|-------|------------|-------------|
| 🔴 Red | >10 GB | Huge directories |
| 🟣 Magenta | >1 GB | Large directories |
| 🟡 Yellow | >100 MB | Medium directories |
| 🔵 Cyan | >10 MB | Small-medium directories |
| 🟢 Green | <10 MB | Small directories |

## Example Use Cases

- **Find storage hogs**: Quickly identify which directories are consuming the most disk space
- **System cleanup**: Locate large directories that can be cleaned up or moved
- **Storage analysis**: Generate professional reports for system administration
- **Backup planning**: Understand directory sizes before creating backups

## Technical Details

- **Language**: C (C99 standard)
- **Dependencies**: Standard C library, POSIX APIs
- **Compatibility**: macOS 10.12+ (may work on other Unix-like systems)
- **Performance**: Efficiently handles thousands of directories
- **Memory**: Fixed memory footprint (supports up to 1000 directories by default)

## Building and Development

### Compile with Debug Information
```bash
gcc -std=c99 -g -o dirscanner dirscanner.c
```

### Compile with Optimizations
```bash
gcc -std=c99 -O2 -o dirscanner dirscanner.c
```

### Code Structure
- `calculate_dir_size()`: Recursive directory size calculation
- `scan_directory()`: Main directory scanning logic
- `generate_html_report()`: HTML report generation
- Color and formatting utilities for enhanced output

## Limitations

- Maximum of 1000 directories per scan (configurable in source)
- Requires read permissions for target directories
- Symbolic links are not followed (uses `lstat`)
- May be slow on very large directory structures

## Contributing

Contributions are welcome! Please feel free to submit a Pull Request. For major changes, please open an issue first to discuss what you would like to change.

### Development Setup
1. Fork the repository
2. Create your feature branch (`git checkout -b feature/AmazingFeature`)
3. Commit your changes (`git commit -m 'Add some AmazingFeature'`)
4. Push to the branch (`git push origin feature/AmazingFeature`)
5. Open a Pull Request

## License

This project is licensed under the MIT License - see the [LICENSE](#license-text) section below for details.

## Author

**Roberto Santacroce**

## Changelog

### v1.0.0
- Initial release
- Basic directory scanning functionality
- Colorful terminal output
- HTML report generation
- Progress indicators
- Size formatting and sorting

## License Text

```
MIT License

Copyright (c) 2025 Roberto Santacroce

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
```

## Support

If you encounter any issues or have questions, please open an issue on the project repository.

---

⭐ If you find this tool useful, please consider giving it a star!
