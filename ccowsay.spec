Name: ccowsay
Version: 0.1.2
Release: alt1

Summary: Minimal C implementation of cowsay

License: GPL-3.0-or-later
Group: Applications/Text
Url: https://github.com/UsamG1t/ccowsay

Source0: %name-%version.tar.gz

BuildRequires: autoconf automake check gcc libcheck-devel libtool make pkgconfig

%description
ccowsay prints a user-provided message in a speech or thought bubble
and renders an ASCII cow template below it.

%prep
%setup

%build
%autoreconf
%configure
%make_build

%install
%makeinstall_std

%check
make check

%files
%_bindir/ccowsay
%_bindir/cowthink
%_includedir/ccowsay.h
%_libdir/*.so*
%dir %_datadir/%name
%dir %_datadir/%name/cows
%_datadir/%name/cows/*.cow
%doc %_docdir/%name/README

%changelog
* Mon May 18 2026 usam <malchevskijsa@basealt.ru> 0.1.2-alt1
- Added cowthink utility and cow rendering tests.
- Added Check/checkmk based library tests.

* Fri May 15 2026 usam <malchevskijsa@basealt.ru> 0.1.1-alt1
- Added libtool library with bundled cow templates.
- Added automake tests and enabled package check.

* Thu May 14 2026 usam <malchevskijsa@basealt.ru> 0.1.0-alt1
- Added autotools build system.
- Switched package build to autotools.

* Mon May 04 2026 usam <malchevskijsa@basealt.ru> 0.1.0-alt1
- Initial C cowsay AltLinux build.
