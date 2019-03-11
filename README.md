# Rootkits Subverting The Windows Kernel book: source code examples

<b>Still Work-in-progress..</b>

I have arranged the code according to its order in the book.<br />
I tested most of the code on Windows XP SP0 (no service packs installed) in a VM.<br />
Sample code includes the following:<br />
1. Basic driver.
2. Creating a file handle and a symlink
3. Loading a driver using the quick and dirty way
4. Loading a driver the correct way (using Service Manager)
5. Decompress a SYS file from the rsrc section
6. Parsing the IDT table
7. CR0-trick to disable memory protection
8. Injecting a DLL using remote threads
9. Hooking SSDT (using it to hide processes' names)
10. Hooking the IDT
11. SYSENTER hook
12. Hooking the IRP major-functions table
13. Hybrid hooking approach
14. Detour patching
15. Jump templates
16. Sample keylogger (as a Layered driver)
17. Grabbing OS version from the Registry
18. Grabbing OS version from Kernel-Mode (OSVERSIONINFO structure)
19. Hiding devices/drivers (using DKOM)
20. Privilege elevation (using DKOM)
21. Playing with the Keyboard LEDs (accessing the hardware directly)
22. Keysniffer (by accessing the hardware directly)<br />
23-27. RootKit Detection methods.


Lessons were put in order ( + some added code) by: [0xbahaa](https://patch8.com)<br />
Forked from the [original mirror by: fdiskyou](https://github.com/fdiskyou/www.rootkit.com)<br />
Links to the book:
- [Google Books](https://books.google.com/books?id=fDxg1W3eT2gC)
- [Amazon](https://www.amazon.com/Rootkits-Subverting-Windows-Greg-Hoglund/dp/0321294319)

