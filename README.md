# ft_ping

**ft_ping** is an application for sending and receiving ICMP (Internet Control Message Protocol) packets. This project works similarly to the standard `ping` command and is used for basic network diagnostics.

---

## Features

### Mandatory Features
- **`-v`**: Provides more detailed information. Prints error conditions and packet details.
- **`-?`**: Prints help information and exits the program.

### Bonus Features
- **`-f`**: Flood mode. Sends continuous and fast pings.
- **`-l <num>`**: Preload mode. Prepares and sends a specified number of packets. (not yet developed)
- **`-n`**: Disables DNS resolution and works directly with IP addresses.
- **`-w <time>`**: Sets a total runtime limit for the program.
- **`-W <time>`**: Specifies the maximum wait time for each ICMP packet response.
- **`--ttl <val>`**: Configures the Time To Live (TTL) value for ICMP packets.
- **`--ip-timestamp`**: Adds a timestamp to the sent ICMP packets.

---

## Installation

1. Clone the repository:
   ```bash
   git clone <repository_url>
   cd ft_ping
   ```
2. Compile the project:
   ```bash
   make
   ```

---

## Usage

### Examples

1. **Basic Ping Operation**:
   ```bash
   sudo ./ft_ping google.com
   ```

2. **Verbose Mode**:
   ```bash
   sudo ./ft_ping google.com -v
   ```

3. **Flood Mode**:
   ```bash
   sudo ./ft_ping google.com -f
   ```

4. **TTL Configuration**:
   ```bash
   sudo ./ft_ping google.com --ttl 5
   ```

5. **Help Information**:
   ```bash
   ./ft_ping -?
   ```

---

## Project Structure

```plaintext
.
├── ft_ping.h           # Header file
├── ft_ping.c           # Main program file
├── is_valid_funcs.c    # Helper functions
├── flags.c             # Flags
├── Makefile            # Makefile
└── README.md           # Project description
```

---

## Known Issues
- When the TTL value is set low, some network devices may not return a "Time Exceeded" message.
- The `-f` flag can cause performance issues due to excessive packet sending.

