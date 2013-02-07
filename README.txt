ReDOS: A simple operating system
--------------------------------

ReDOS is designed to be built in Visual Studio 10.
Master branch should build as-is from included .sln file.

BUILD STEPS
    1. Download and uncompress source into a directory
    2. Load ReDOS.sln into Visual Studio
    3. Build in RELEASE mode (Debug mode WILL NOT WORK)
    4. Kernel is located in Release\ReDOS.exe

RUN STEPS
    1. In the 'run' subfolder are the neededed files for bochs
    2. You must transfer the kernel into a grub image; the base_grub.img is
       correctly set up for this purpose.  I know of two ways to transfer the
       image:
        A) Use powerISO available from http://www.poweriso.com/ (shareware)
        B) Use loopback mouning on a *nix system. (NOTE: Cygwin does not
           support loopback mounting! Save yourself the trouble!)
       Save ReDOS.exe into the root directory on the disk image.
    3. Now run Bochs using the included bochsrc.bxrc (Obtain Bochs from
       http://bochs.sourceforge.net/ or your local repository)
