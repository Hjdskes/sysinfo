# Maintainer: Unia <jthidskes@outlook.com> 

pkgname=sysinfo-git
_gitname=sysinfo
pkgver=2013.04.10
pkgrel=1
pkgdesc="A tool to display theme and system information to be used in screenshots"
arch=('i686' 'x86_64')
url="https://github.com/Unia/sysinfo"
license=('GPL2')
depends=('pacman' 'glib2')
makedepends=('git')
source=('git://github.com/Unia/sysinfo.git')
md5sums=('SKIP')

pkgver() {
  cd $_gitname
  git log -1 --format="%cd" --date=short | sed 's|-|.|g'
}

build() {
  cd $_gitname
  make
}

package() {
  cd $_gitname
  make PREFIX=/usr DESTDIR="$pkgdir" install
}
