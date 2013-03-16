pkgname=sysinfo-git
pkgbase=sysinfo
pkgver=20130305
pkgrel=1
pkgdesc="A tool to display theme and system information to be used in screenshots"
arch=(any)
groups=(custom)
url="https://github.com/Unia/sysinfo"
license=(GPL2)
depends=('pacman' 'glib2')
conflicts=('runescape-client-bin' 'unix-runescape-client')

_gitroot="https://github.com/Unia/$pkgbase"
_gitname="$pkgbase"

build() {
  cd "$srcdir"
  msg "Connecting to GIT server..."

  if [ -d ${_gitname} ] ; then
    cd ${_gitname}
    git pull
    msg "The local files are updated."
  else
    git clone ${_gitroot} ${_gitname}
  fi
  msg "GIT checkout done or server timeout"
}

package() {
  cd "$srcdir/$pkgbase"
  make
  make DESTDIR="$pkgdir" install
}
md5sums=()
