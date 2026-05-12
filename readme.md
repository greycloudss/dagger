
# Dagger

**Dagger** is a lightweight C++ packet inspection tool focused on **live interface capture**, **raw frame parsing**, and **readable protocol output**.  
It is built around `libpcap` and direct byte-level parsing, with minimal abstraction and explicit control over packet decoding.

> Work in progress: internals and parsing behavior may change across minor versions until the project stabilizes.  
> Part of the **Armour series**
---

## Overview

Dagger captures live traffic from a selected network adapter, parses raw packet bytes, and formats decoded fields into readable terminal output.

The project is intentionally low-level. Packet fields are extracted directly from raw byte buffers rather than through higher-level networking frameworks.

Core areas include:

| Module / Area       | Purpose                                                                    |
| :------------------ | :------------------------------------------------------------------------- |
| **adapter/capture** | Finds adapters, selects loopback or connection device, opens live capture  |
| **packet/**         | Parses raw Ethernet and IPv4 headers from captured bytes                   |
| **formatting/**     | Converts parsed fields into readable terminal output                       |
| **protocol map**    | Resolves protocol IDs into protocol names                                  |
| **ttl analysis**    | Performs simple TTL-based OS guessing                                      |

---

## Design Goals

- **Low-level parsing** — packet fields are read directly from raw byte arrays
- **Minimal dependencies** — relies mainly on standard C++ and `libpcap`
- **Readable output** — packet contents are formatted into structured terminal text
- **Interactive capture** — supports adapter selection before capture begins
- **Incremental expansion** — additional protocol decoders can be added without rewriting capture flow

---

## Technical Summary

| Area                | Implementation                                     |
| ------------------- | -------------------------------------------------- |
| **Language**        | C++                                                |
| **Runtime**         | Linux                                              |
| **Capture backend** | `libpcap`                                          |
| **Current parsing** | Ethernet, IPv4                                     |
| **Output**          | ANSI-colored terminal text                         |
| **Selection model** | Loopback or default-route adapter selection        |

---

## Current Capabilities

* Detects and opens capture-capable adapters through `pcap_findalldevs`
* Supports loopback capture
* Supports default-route adapter selection
* Captures live packets using `pcap_open_live`
* Parses Ethernet headers
* Parses IPv4 headers
* Prints source and destination addressing information
* Resolves IPv4 protocol numbers into readable protocol names
* Performs rough TTL-based OS guessing
* Displays raw packet bytes alongside parsed output

---

## Example Output

```text
[PACKET] packet size: 98
Destination MAC: 00:00:00:00:00:00
Source MAC: 00:00:00:00:00:00
EtherType: 0x0800
Version: 4
IHL: 5
Service Type: 0
Total Length: 84
Identification: 64677
Flags: 2
Fragment Offset: 0
TTL: 64
OS Guess: Linux/FreeBSD/macOS/Unix-like
Protocol: ICMP (1)
Header Checksum: 16385
Source: 127.0.0.1
Destination: 127.0.0.1
```

---

## Building

### Requirements

* Linux
* `g++`
* `libpcap`
* Root privileges or equivalent capture permissions

### Example

```bash
g++ -std=c++20 sharking.cpp main.cpp -lpcap -o dagger
```

---

## Running

```bash
sudo ./dagger
```

At startup, the program can be configured to:

* listen on the loopback adapter
* listen on the default connection adapter

---

## Developer Notes

* Packet decoding currently assumes Ethernet framing before IPv4 parsing
* Loopback traffic may expose synthetic or zeroed MAC addresses depending on capture format
* TTL-based OS identification is heuristic only and should not be treated as reliable fingerprinting
* Raw packet ownership and capture buffering need careful handling to avoid lifetime and cleanup bugs

---

## License

This project is provided for research and educational purposes.
No warranty is provided. Use on systems you own or are authorized to test.

© 2026 greycloudss — All Rights Reserved.
