# Network Diagnostic Tool

A simple C program that gathers network information from Windows computers for IT support troubleshooting.

## Features

This tool executes the following network commands:

- **IP Configuration** - `ipconfig /all` - Full network adapter details
- **Network Interface Status** - Interface connection status
- **Routing Table** - `route print` - Network routing information
- **ARP Table** - `arp -a` - Address Resolution Protocol cache
- **DNS Test** - `nslookup google.com` - DNS resolution test
- **Network Statistics** - `netstat -an` - Active connections
- **Network Adapter Details** - Hardware and speed info
- **Wireless Info** - `netsh wlan show interfaces` - WiFi details
- **Windows Firewall Status** - Security profile status
- **Proxy Settings** - System proxy configuration
- **Ping Tests**:
  - Loopback (127.0.0.1) - Local network stack test
  - Google DNS (8.8.8.8) - Internet connectivity test
  - Google.com - DNS + Internet test


## Usage

### Run the executable directly
```
network_tool.exe
```

## Output

The tool generates a `network_report.txt` file with timestamp and all command outputs, then automatically opens it in Notepad.

## Building from Source

If you need to compile from source:

```bash
gcc -o network_tool.exe network_tool.c
```

Or with MSVC:
```bash
cl network_tool.c
```

## Requirements

- Windows OS (Windows 7 or later recommended)
- No external dependencies required for the compiled executable

## For IT Support

This tool is designed to be:
- **Simple**: Just double-click and wait
- **Comprehensive**: Covers common network diagnostic areas
- **Portable**: Single .exe file, no installation needed
- **User-friendly**: Automatically opens the report when done

Users can run this and send you the generated `network_report.txt` file for analysis.
