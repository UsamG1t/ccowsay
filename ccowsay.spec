Name: ccowsay
Version: 0.1.0
Release: alt2

Summary: Minimal C implementation of cowsay

License: GPL-3.0-or-later
Group: Applications/Text
Url: https://github.com/UsamG1t/ccowsay

Source0: %name-%version.tar.gz

BuildRequires: autoconf automake gcc make

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

%files
%_bindir/ccowsay
%dir %_datadir/%name
%dir %_datadir/%name/cows
%_datadir/%name/cows/*.cow
%doc %_docdir/%name/README

%changelog
* Thu May 14 2026 usam <malchevskijsa@basealt.ru> 0.1.0-alt2
- Added autotools build system.
- Switched package build to autotools.

* Mon May 04 2026 usam <malchevskijsa@basealt.ru> 0.1.0-alt1
- Initial C cowsay AltLinux build.
