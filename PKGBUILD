pkgname=sysinfo-git
pkgbase=sysinfo
pkgver=20130316
pkgrel=1
pkgdesc="A tool to display theme and system information to be used in screenshots"
arch=(any)
url="https://github.com/Unia/sysinfo"
license=(GPL2)
depends=('pacman' 'glib2')
md5sums=()

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

	make
}

package() {
	cd "$srcdir/$pkgbase"
	make DESTDIR="$pkgdir" install
}
