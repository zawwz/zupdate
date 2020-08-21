# zupdate

Cross-distro updating and safe update preview

## Installing

### From [zpkg](https://github.com/zawwz/zpkg)

```sh
# install zpkg
wget -qO- https://zpkg.zawz.net/install.sh | sh
# install zupdate
zpkg install zupdate
```

### From source

#### Dependencies

Dependencies:  
- [ztd](https://github.com/zawwz/ztd)  
- sudo

Supported package managers:  
- pacman  
- apt  

Optional dependencies:  
- yay (AUR support for arch)  

#### Building

``make -j8`` for a shared build  
``STATIC=true make -j8`` for a static build  

## Usage

Full update preview: `zupdate`  
Upgrade: `zupdate -yu`  

Parsable package list: `zupdate -L`  
Download size: `zupdate -kd`  

See `zupdate -h` for full details
