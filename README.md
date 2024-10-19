-----

logo here

-----

### NRPM

**NRPM**, or Nix Reproducible Package Manager, is an idea I had (shocking I know), about making `nix-env` reproducible. There might be an already existing tool that does this, but as far as I know, I'm the first to do this the way I'm doing.

-----

- `What is nrpm?`:
	`nrpm` or `nix reproducible package manager`, is an excuse of mine to make a project I had made in bash in C++. But really what it is (as I see it), it's an `nix-env` alternative that is reproducible.

- `What do I mean, "reproducible"?`:
	  Unlike `nix-env`, which installs packages in an way that you'd have to manually reinstall them if you were to build your NixOS again, `nrpm` will actually make a `packages.nix` file that includes `home.packages` and `home.programs`, essentially its a imperative way to declare your packages for home-manager. 

- `Why?`:
	  I am a fan of the imperative way of installing packages on other Linux distributions, and to this day, I hate having to edit a file (even though that's the point of NixOS) just to install a package, so I made `nixpm`, a bash script that, is essentially a simpler version of this program.

----

### Setting up

I haven't actually worked on the whole nixos installation process, nor have I finished the code, so when I actually do, I'll update this part.

----

### Contributing

If you think my code is shit, it's because it most likely is, and if you have any advice for me on how to improve my code, please reach out on discord <actually.auth>, or create an issue on this repo.
