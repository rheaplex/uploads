; Puzzlebox Synapse Windows Installer

; NOTE: this .NSI script is designed for NSIS v1.8+

Name "Puzzlebox Synapse"
OutFile "Puzzlebox-Synapse-0.4.3-setup.exe"

; Some default compiler settings (uncomment and change at will):
; SetCompress auto ; (can be off or force)
; SetDatablockOptimize on ; (can be off)
; CRCCheck on ; (can be off)
; AutoCloseWindow false ; (can be true for the window go away automatically at end)
; ShowInstDetails hide ; (can be show to have them shown, or nevershow to disable)
; SetDateSave off ; (can be on to have files restored to their orginal date)

LicenseText "You must agree to this license before installing."
LicenseData "LICENSE.txt"

InstallDir "$PROGRAMFILES\Puzzlebox Synapse"
InstallDirRegKey HKEY_LOCAL_MACHINE "SOFTWARE\Puzzlebox Synapse\Puzzlebox Synapse" ""
;DirShow show ; (make this hide to not let the user change it)
DirText "Select the directory to install Puzzlebox Synapse into:"

InstallColors /windows

Section "" ; (default section)
WriteRegStr HKEY_LOCAL_MACHINE "SOFTWARE\Puzzlebox Synapse\Puzzlebox Synapse" "" "$INSTDIR"
WriteRegStr HKEY_LOCAL_MACHINE "Software\Microsoft\Windows\CurrentVersion\Uninstall\Puzzlebox Synapse\Puzzlebox Synapse" "DisplayName" "Puzzlebox Synapse (remove only)"
WriteRegStr HKEY_LOCAL_MACHINE "Software\Microsoft\Windows\CurrentVersion\Uninstall\Puzzlebox Synapse\Puzzlebox Synapse" "UninstallString" '"$INSTDIR\Puzzlesbox-Synapse-Uninstall.exe"'


SetOutPath "$INSTDIR"
 File dist\msvcr71.dll
 File dist\puzzlebox_synapse_configuration.ini
 File dist\python25.dll
 File dist\synapse-client.exe
 File dist\synapse-gui.exe
 File dist\synapse-protocol.exe
 File dist\synapse-server.exe
 File dist\w9xpopen.exe


SetOutPath $INSTDIR\lib
 File dist\lib\library.zip


; File dist\lib\tcl\tcl8.4\auto.tcl
; File dist\lib\tcl\tcl8.4\encoding\ascii.enc
; File dist\lib\tcl\tcl8.4\encoding\big5.enc
; File dist\lib\tcl\tcl8.4\encoding\cp1250.enc
; File dist\lib\tcl\tcl8.4\encoding\cp1251.enc
; File dist\lib\tcl\tcl8.4\encoding\cp1252.enc
; File dist\lib\tcl\tcl8.4\encoding\cp1253.enc
; File dist\lib\tcl\tcl8.4\encoding\cp1254.enc
; File dist\lib\tcl\tcl8.4\encoding\cp1255.enc
; File dist\lib\tcl\tcl8.4\encoding\cp1256.enc
; File dist\lib\tcl\tcl8.4\encoding\cp1257.enc
; File dist\lib\tcl\tcl8.4\encoding\cp1258.enc
; File dist\lib\tcl\tcl8.4\encoding\cp437.enc
; File dist\lib\tcl\tcl8.4\encoding\cp737.enc
; File dist\lib\tcl\tcl8.4\encoding\cp775.enc
; File dist\lib\tcl\tcl8.4\encoding\cp850.enc
; File dist\lib\tcl\tcl8.4\encoding\cp852.enc
; File dist\lib\tcl\tcl8.4\encoding\cp855.enc
; File dist\lib\tcl\tcl8.4\encoding\cp857.enc
; File dist\lib\tcl\tcl8.4\encoding\cp860.enc
; File dist\lib\tcl\tcl8.4\encoding\cp861.enc
; File dist\lib\tcl\tcl8.4\encoding\cp862.enc
; File dist\lib\tcl\tcl8.4\encoding\cp863.enc
; File dist\lib\tcl\tcl8.4\encoding\cp864.enc
; File dist\lib\tcl\tcl8.4\encoding\cp865.enc
; File dist\lib\tcl\tcl8.4\encoding\cp866.enc
; File dist\lib\tcl\tcl8.4\encoding\cp869.enc
; File dist\lib\tcl\tcl8.4\encoding\cp874.enc
; File dist\lib\tcl\tcl8.4\encoding\cp932.enc
; File dist\lib\tcl\tcl8.4\encoding\cp936.enc
; File dist\lib\tcl\tcl8.4\encoding\cp949.enc
; File dist\lib\tcl\tcl8.4\encoding\cp950.enc
; File dist\lib\tcl\tcl8.4\encoding\dingbats.enc
; File dist\lib\tcl\tcl8.4\encoding\ebcdic.enc
; File dist\lib\tcl\tcl8.4\encoding\euc-cn.enc
; File dist\lib\tcl\tcl8.4\encoding\euc-jp.enc
; File dist\lib\tcl\tcl8.4\encoding\euc-kr.enc
; File dist\lib\tcl\tcl8.4\encoding\gb12345.enc
; File dist\lib\tcl\tcl8.4\encoding\gb1988.enc
; File dist\lib\tcl\tcl8.4\encoding\gb2312.enc
; File dist\lib\tcl\tcl8.4\encoding\gb2312-raw.enc
; File dist\lib\tcl\tcl8.4\encoding\iso2022.enc
; File dist\lib\tcl\tcl8.4\encoding\iso2022-jp.enc
; File dist\lib\tcl\tcl8.4\encoding\iso2022-kr.enc
; File dist\lib\tcl\tcl8.4\encoding\iso8859-10.enc
; File dist\lib\tcl\tcl8.4\encoding\iso8859-13.enc
; File dist\lib\tcl\tcl8.4\encoding\iso8859-14.enc
; File dist\lib\tcl\tcl8.4\encoding\iso8859-15.enc
; File dist\lib\tcl\tcl8.4\encoding\iso8859-16.enc
; File dist\lib\tcl\tcl8.4\encoding\iso8859-1.enc
; File dist\lib\tcl\tcl8.4\encoding\iso8859-2.enc
; File dist\lib\tcl\tcl8.4\encoding\iso8859-3.enc
; File dist\lib\tcl\tcl8.4\encoding\iso8859-4.enc
; File dist\lib\tcl\tcl8.4\encoding\iso8859-5.enc
; File dist\lib\tcl\tcl8.4\encoding\iso8859-6.enc
; File dist\lib\tcl\tcl8.4\encoding\iso8859-7.enc
; File dist\lib\tcl\tcl8.4\encoding\iso8859-8.enc
; File dist\lib\tcl\tcl8.4\encoding\iso8859-9.enc
; File dist\lib\tcl\tcl8.4\encoding\jis0201.enc
; File dist\lib\tcl\tcl8.4\encoding\jis0208.enc
; File dist\lib\tcl\tcl8.4\encoding\jis0212.enc
; File dist\lib\tcl\tcl8.4\encoding\koi8-r.enc
; File dist\lib\tcl\tcl8.4\encoding\koi8-u.enc
; File dist\lib\tcl\tcl8.4\encoding\ksc5601.enc
; File dist\lib\tcl\tcl8.4\encoding\macCentEuro.enc
; File dist\lib\tcl\tcl8.4\encoding\macCroatian.enc
; File dist\lib\tcl\tcl8.4\encoding\macCyrillic.enc
; File dist\lib\tcl\tcl8.4\encoding\macDingbats.enc
; File dist\lib\tcl\tcl8.4\encoding\macGreek.enc
; File dist\lib\tcl\tcl8.4\encoding\macIceland.enc
; File dist\lib\tcl\tcl8.4\encoding\macJapan.enc
; File dist\lib\tcl\tcl8.4\encoding\macRoman.enc
; File dist\lib\tcl\tcl8.4\encoding\macRomania.enc
; File dist\lib\tcl\tcl8.4\encoding\macThai.enc
; File dist\lib\tcl\tcl8.4\encoding\macTurkish.enc
; File dist\lib\tcl\tcl8.4\encoding\macUkraine.enc
; File dist\lib\tcl\tcl8.4\encoding\shiftjis.enc
; File dist\lib\tcl\tcl8.4\encoding\symbol.enc
; File dist\lib\tcl\tcl8.4\encoding\tis-620.enc
; File dist\lib\tcl\tcl8.4\history.tcl
; File dist\lib\tcl\tcl8.4\http1.0\http.tcl
; File dist\lib\tcl\tcl8.4\http1.0\pkgIndex.tcl
; File dist\lib\tcl\tcl8.4\http2.5\http.tcl
; File dist\lib\tcl\tcl8.4\http2.5\pkgIndex.tcl
; File dist\lib\tcl\tcl8.4\init.tcl
; File dist\lib\tcl\tcl8.4\ldAout.tcl
; File dist\lib\tcl\tcl8.4\msgcat1.3\msgcat.tcl
; File dist\lib\tcl\tcl8.4\msgcat1.3\pkgIndex.tcl
; File dist\lib\tcl\tcl8.4\opt0.4\optparse.tcl
; File dist\lib\tcl\tcl8.4\opt0.4\pkgIndex.tcl
; File dist\lib\tcl\tcl8.4\package.tcl
; File dist\lib\tcl\tcl8.4\parray.tcl
; File dist\lib\tcl\tcl8.4\safe.tcl
; File dist\lib\tcl\tcl8.4\tclIndex
; File dist\lib\tcl\tcl8.4\tcltest2.2\pkgIndex.tcl
; File dist\lib\tcl\tcl8.4\tcltest2.2\tcltest.tcl
; File dist\lib\tcl\tcl8.4\word.tcl
; File dist\lib\tcl\tk8.4\bgerror.tcl
; File dist\lib\tcl\tk8.4\button.tcl
; File dist\lib\tcl\tk8.4\choosedir.tcl
; File dist\lib\tcl\tk8.4\clrpick.tcl
; File dist\lib\tcl\tk8.4\comdlg.tcl
; File dist\lib\tcl\tk8.4\console.tcl
; File dist\lib\tcl\tk8.4\demos\arrow.tcl
; File dist\lib\tcl\tk8.4\demos\bind.tcl
; File dist\lib\tcl\tk8.4\demos\bitmap.tcl
; File dist\lib\tcl\tk8.4\demos\browse
; File dist\lib\tcl\tk8.4\demos\button.tcl
; File dist\lib\tcl\tk8.4\demos\check.tcl
; File dist\lib\tcl\tk8.4\demos\clrpick.tcl
; File dist\lib\tcl\tk8.4\demos\colors.tcl
; File dist\lib\tcl\tk8.4\demos\cscroll.tcl
; File dist\lib\tcl\tk8.4\demos\ctext.tcl
; File dist\lib\tcl\tk8.4\demos\dialog1.tcl
; File dist\lib\tcl\tk8.4\demos\dialog2.tcl
; File dist\lib\tcl\tk8.4\demos\entry1.tcl
; File dist\lib\tcl\tk8.4\demos\entry2.tcl
; File dist\lib\tcl\tk8.4\demos\entry3.tcl
; File dist\lib\tcl\tk8.4\demos\filebox.tcl
; File dist\lib\tcl\tk8.4\demos\floor.tcl
; File dist\lib\tcl\tk8.4\demos\form.tcl
; File dist\lib\tcl\tk8.4\demos\hello
; File dist\lib\tcl\tk8.4\demos\hscale.tcl
; File dist\lib\tcl\tk8.4\demos\icon.tcl
; File dist\lib\tcl\tk8.4\demos\image1.tcl
; File dist\lib\tcl\tk8.4\demos\image2.tcl
; File dist\lib\tcl\tk8.4\demos\images\earth.gif
; File dist\lib\tcl\tk8.4\demos\images\earthris.gif
; File dist\lib\tcl\tk8.4\demos\images\face.bmp
; File dist\lib\tcl\tk8.4\demos\images\flagdown.bmp
; File dist\lib\tcl\tk8.4\demos\images\flagup.bmp
; File dist\lib\tcl\tk8.4\demos\images\gray25.bmp
; File dist\lib\tcl\tk8.4\demos\images\letters.bmp
; File dist\lib\tcl\tk8.4\demos\images\noletter.bmp
; File dist\lib\tcl\tk8.4\demos\images\pattern.bmp
; File dist\lib\tcl\tk8.4\demos\images\tcllogo.gif
; File dist\lib\tcl\tk8.4\demos\images\teapot.ppm
; File dist\lib\tcl\tk8.4\demos\items.tcl
; File dist\lib\tcl\tk8.4\demos\ixset
; File dist\lib\tcl\tk8.4\demos\labelframe.tcl
; File dist\lib\tcl\tk8.4\demos\label.tcl
; File dist\lib\tcl\tk8.4\demos\license.terms
; File dist\lib\tcl\tk8.4\demos\menubu.tcl
; File dist\lib\tcl\tk8.4\demos\menu.tcl
; File dist\lib\tcl\tk8.4\demos\msgbox.tcl
; File dist\lib\tcl\tk8.4\demos\paned1.tcl
; File dist\lib\tcl\tk8.4\demos\paned2.tcl
; File dist\lib\tcl\tk8.4\demos\plot.tcl
; File dist\lib\tcl\tk8.4\demos\puzzle.tcl
; File dist\lib\tcl\tk8.4\demos\radio.tcl
; File dist\lib\tcl\tk8.4\demos\README
; File dist\lib\tcl\tk8.4\demos\rmt
; File dist\lib\tcl\tk8.4\demos\rolodex
; File dist\lib\tcl\tk8.4\demos\ruler.tcl
; File dist\lib\tcl\tk8.4\demos\sayings.tcl
; File dist\lib\tcl\tk8.4\demos\search.tcl
; File dist\lib\tcl\tk8.4\demos\spin.tcl
; File dist\lib\tcl\tk8.4\demos\square
; File dist\lib\tcl\tk8.4\demos\states.tcl
; File dist\lib\tcl\tk8.4\demos\style.tcl
; File dist\lib\tcl\tk8.4\demos\tclIndex
; File dist\lib\tcl\tk8.4\demos\tcolor
; File dist\lib\tcl\tk8.4\demos\text.tcl
; File dist\lib\tcl\tk8.4\demos\timer
; File dist\lib\tcl\tk8.4\demos\twind.tcl
; File dist\lib\tcl\tk8.4\demos\unicodeout.tcl
; File dist\lib\tcl\tk8.4\demos\vscale.tcl
; File dist\lib\tcl\tk8.4\demos\widget
; File dist\lib\tcl\tk8.4\dialog.tcl
; File dist\lib\tcl\tk8.4\entry.tcl
; File dist\lib\tcl\tk8.4\focus.tcl
; File dist\lib\tcl\tk8.4\images\logo100.gif
; File dist\lib\tcl\tk8.4\images\logo64.gif
; File dist\lib\tcl\tk8.4\images\logo.eps
; File dist\lib\tcl\tk8.4\images\logoLarge.gif
; File dist\lib\tcl\tk8.4\images\logoMed.gif
; File dist\lib\tcl\tk8.4\images\pwrdLogo100.gif
; File dist\lib\tcl\tk8.4\images\pwrdLogo150.gif
; File dist\lib\tcl\tk8.4\images\pwrdLogo175.gif
; File dist\lib\tcl\tk8.4\images\pwrdLogo200.gif
; File dist\lib\tcl\tk8.4\images\pwrdLogo75.gif
; File dist\lib\tcl\tk8.4\images\pwrdLogo.eps
; File dist\lib\tcl\tk8.4\images\README
; File dist\lib\tcl\tk8.4\images\tai-ku.gif
; File dist\lib\tcl\tk8.4\license.terms
; File dist\lib\tcl\tk8.4\listbox.tcl
; File dist\lib\tcl\tk8.4\menu.tcl
; File dist\lib\tcl\tk8.4\mkpsenc.tcl
; File dist\lib\tcl\tk8.4\msgbox.tcl
; File dist\lib\tcl\tk8.4\msgs\cs.msg
; File dist\lib\tcl\tk8.4\msgs\de.msg
; File dist\lib\tcl\tk8.4\msgs\el.msg
; File dist\lib\tcl\tk8.4\msgs\en_gb.msg
; File dist\lib\tcl\tk8.4\msgs\en.msg
; File dist\lib\tcl\tk8.4\msgs\eo.msg
; File dist\lib\tcl\tk8.4\msgs\es_ES.msg
; File dist\lib\tcl\tk8.4\msgs\es.msg
; File dist\lib\tcl\tk8.4\msgs\fr.msg
; File dist\lib\tcl\tk8.4\msgs\it.msg
; File dist\lib\tcl\tk8.4\msgs\nl.msg
; File dist\lib\tcl\tk8.4\msgs\pl.msg
; File dist\lib\tcl\tk8.4\msgs\ru.msg
; File dist\lib\tcl\tk8.4\obsolete.tcl
; File dist\lib\tcl\tk8.4\optMenu.tcl
; File dist\lib\tcl\tk8.4\palette.tcl
; File dist\lib\tcl\tk8.4\panedwindow.tcl
; File dist\lib\tcl\tk8.4\pkgIndex.tcl
; File dist\lib\tcl\tk8.4\safetk.tcl
; File dist\lib\tcl\tk8.4\scale.tcl
; File dist\lib\tcl\tk8.4\scrlbar.tcl
; File dist\lib\tcl\tk8.4\spinbox.tcl
; File dist\lib\tcl\tk8.4\tclIndex
; File dist\lib\tcl\tk8.4\tearoff.tcl
; File dist\lib\tcl\tk8.4\text.tcl
; File dist\lib\tcl\tk8.4\tkfbox.tcl
; File dist\lib\tcl\tk8.4\tk.tcl
; File dist\lib\tcl\tk8.4\unsupported.tcl
; File dist\lib\tcl\tk8.4\xmfbox.tcl


SetOutPath $INSTDIR\mpl-data
 File dist\mpl-data\lineprops.glade
 File dist\mpl-data\matplotlib.conf
 File dist\mpl-data\matplotlibrc


SetOutPath $INSTDIR\mpl-data\fonts\afm
 File dist\mpl-data\fonts\afm\cmex10.afm
 File dist\mpl-data\fonts\afm\cmmi10.afm
 File dist\mpl-data\fonts\afm\cmr10.afm
 File dist\mpl-data\fonts\afm\cmsy10.afm
 File dist\mpl-data\fonts\afm\cmtt10.afm
 File dist\mpl-data\fonts\afm\pagd8a.afm
 File dist\mpl-data\fonts\afm\pagdo8a.afm
 File dist\mpl-data\fonts\afm\pagk8a.afm
 File dist\mpl-data\fonts\afm\pagko8a.afm
 File dist\mpl-data\fonts\afm\pbkd8a.afm
 File dist\mpl-data\fonts\afm\pbkdi8a.afm
 File dist\mpl-data\fonts\afm\pbkl8a.afm
 File dist\mpl-data\fonts\afm\pbkli8a.afm
 File dist\mpl-data\fonts\afm\pcrb8a.afm
 File dist\mpl-data\fonts\afm\pcrbo8a.afm
 File dist\mpl-data\fonts\afm\pcrr8a.afm
 File dist\mpl-data\fonts\afm\pcrro8a.afm
 File dist\mpl-data\fonts\afm\phvb8a.afm
 File dist\mpl-data\fonts\afm\phvb8an.afm
 File dist\mpl-data\fonts\afm\phvbo8a.afm
 File dist\mpl-data\fonts\afm\phvbo8an.afm
 File dist\mpl-data\fonts\afm\phvl8a.afm
 File dist\mpl-data\fonts\afm\phvlo8a.afm
 File dist\mpl-data\fonts\afm\phvr8a.afm
 File dist\mpl-data\fonts\afm\phvr8an.afm
 File dist\mpl-data\fonts\afm\phvro8a.afm
 File dist\mpl-data\fonts\afm\phvro8an.afm
 File dist\mpl-data\fonts\afm\pncb8a.afm
 File dist\mpl-data\fonts\afm\pncbi8a.afm
 File dist\mpl-data\fonts\afm\pncr8a.afm
 File dist\mpl-data\fonts\afm\pncri8a.afm
 File dist\mpl-data\fonts\afm\pplb8a.afm
 File dist\mpl-data\fonts\afm\pplbi8a.afm
 File dist\mpl-data\fonts\afm\pplr8a.afm
 File dist\mpl-data\fonts\afm\pplri8a.afm
 File dist\mpl-data\fonts\afm\psyr.afm
 File dist\mpl-data\fonts\afm\ptmb8a.afm
 File dist\mpl-data\fonts\afm\ptmbi8a.afm
 File dist\mpl-data\fonts\afm\ptmr8a.afm
 File dist\mpl-data\fonts\afm\ptmri8a.afm
 File dist\mpl-data\fonts\afm\putb8a.afm
 File dist\mpl-data\fonts\afm\putbi8a.afm
 File dist\mpl-data\fonts\afm\putr8a.afm
 File dist\mpl-data\fonts\afm\putri8a.afm
 File dist\mpl-data\fonts\afm\pzcmi8a.afm
 File dist\mpl-data\fonts\afm\pzdr.afm


SetOutPath $INSTDIR\mpl-data\fonts\pdfcorefonts
 File dist\mpl-data\fonts\pdfcorefonts\Courier.afm
 File dist\mpl-data\fonts\pdfcorefonts\Courier-Bold.afm
 File dist\mpl-data\fonts\pdfcorefonts\Courier-BoldOblique.afm
 File dist\mpl-data\fonts\pdfcorefonts\Courier-Oblique.afm
 File dist\mpl-data\fonts\pdfcorefonts\Helvetica.afm
 File dist\mpl-data\fonts\pdfcorefonts\Helvetica-Bold.afm
 File dist\mpl-data\fonts\pdfcorefonts\Helvetica-BoldOblique.afm
 File dist\mpl-data\fonts\pdfcorefonts\Helvetica-Oblique.afm
 File dist\mpl-data\fonts\pdfcorefonts\readme.txt
 File dist\mpl-data\fonts\pdfcorefonts\Symbol.afm
 File dist\mpl-data\fonts\pdfcorefonts\Times-Bold.afm
 File dist\mpl-data\fonts\pdfcorefonts\Times-BoldItalic.afm
 File dist\mpl-data\fonts\pdfcorefonts\Times-Italic.afm
 File dist\mpl-data\fonts\pdfcorefonts\Times-Roman.afm
 File dist\mpl-data\fonts\pdfcorefonts\ZapfDingbats.afm


SetOutPath $INSTDIR\mpl-data\fonts\ttf
 File dist\mpl-data\fonts\ttf\cmb10.ttf
 File dist\mpl-data\fonts\ttf\cmex10.ttf
 File dist\mpl-data\fonts\ttf\cmmi10.ttf
 File dist\mpl-data\fonts\ttf\cmr10.ttf
 File dist\mpl-data\fonts\ttf\cmss10.ttf
 File dist\mpl-data\fonts\ttf\cmsy10.ttf
 File dist\mpl-data\fonts\ttf\cmtt10.ttf
 File dist\mpl-data\fonts\ttf\COPYRIGHT.TXT
 File dist\mpl-data\fonts\ttf\LICENSE_STIX
 File dist\mpl-data\fonts\ttf\README.TXT
 File dist\mpl-data\fonts\ttf\RELEASENOTES.TXT
 File dist\mpl-data\fonts\ttf\STIXGeneralBolIta.ttf
 File dist\mpl-data\fonts\ttf\STIXGeneralBol.ttf
 File dist\mpl-data\fonts\ttf\STIXGeneralItalic.ttf
 File dist\mpl-data\fonts\ttf\STIXGeneral.ttf
 File dist\mpl-data\fonts\ttf\STIXNonUniBolIta.ttf
 File dist\mpl-data\fonts\ttf\STIXNonUniBol.ttf
 File dist\mpl-data\fonts\ttf\STIXNonUniIta.ttf
 File dist\mpl-data\fonts\ttf\STIXNonUni.ttf
 File dist\mpl-data\fonts\ttf\STIXSizFiveSymReg.ttf
 File dist\mpl-data\fonts\ttf\STIXSizFourSymBol.ttf
 File dist\mpl-data\fonts\ttf\STIXSizFourSymReg.ttf
 File dist\mpl-data\fonts\ttf\STIXSizOneSymBol.ttf
 File dist\mpl-data\fonts\ttf\STIXSizOneSymReg.ttf
 File dist\mpl-data\fonts\ttf\STIXSizThreeSymBol.ttf
 File dist\mpl-data\fonts\ttf\STIXSizThreeSymReg.ttf
 File dist\mpl-data\fonts\ttf\STIXSizTwoSymBol.ttf
 File dist\mpl-data\fonts\ttf\STIXSizTwoSymReg.ttf
 File dist\mpl-data\fonts\ttf\VeraBd.ttf
 File dist\mpl-data\fonts\ttf\VeraBI.ttf
 File dist\mpl-data\fonts\ttf\VeraIt.ttf
 File dist\mpl-data\fonts\ttf\VeraMoBd.ttf
 File dist\mpl-data\fonts\ttf\VeraMoBI.ttf
 File dist\mpl-data\fonts\ttf\VeraMoIt.ttf
 File dist\mpl-data\fonts\ttf\VeraMono.ttf
 File dist\mpl-data\fonts\ttf\VeraSeBd.ttf
 File dist\mpl-data\fonts\ttf\VeraSe.ttf
 File dist\mpl-data\fonts\ttf\Vera.ttf


SetOutPath $INSTDIR\mpl-data\images
 File dist\mpl-data\images\back.png
 File dist\mpl-data\images\back.ppm
 File dist\mpl-data\images\back.svg
 File dist\mpl-data\images\back.xpm
 File dist\mpl-data\images\filesave.png
 File dist\mpl-data\images\filesave.ppm
 File dist\mpl-data\images\filesave.svg
 File dist\mpl-data\images\filesave.xpm
 File dist\mpl-data\images\forward.png
 File dist\mpl-data\images\forward.ppm
 File dist\mpl-data\images\forward.svg
 File dist\mpl-data\images\forward.xpm
 File dist\mpl-data\images\hand.png
 File dist\mpl-data\images\hand.ppm
 File dist\mpl-data\images\hand.svg
 File dist\mpl-data\images\hand.xpm
 File dist\mpl-data\images\home.png
 File dist\mpl-data\images\home.ppm
 File dist\mpl-data\images\home.svg
 File dist\mpl-data\images\home.xpm
 File dist\mpl-data\images\matplotlib.png
 File dist\mpl-data\images\matplotlib.svg
 File dist\mpl-data\images\move.png
 File dist\mpl-data\images\move.ppm
 File dist\mpl-data\images\move.svg
 File dist\mpl-data\images\move.xpm
 File dist\mpl-data\images\qt4_editor_options.svg
 File dist\mpl-data\images\stock_close.ppm
 File dist\mpl-data\images\stock_close.xpm
 File dist\mpl-data\images\stock_down.ppm
 File dist\mpl-data\images\stock_down.xpm
 File dist\mpl-data\images\stock_left.ppm
 File dist\mpl-data\images\stock_left.xpm
 File dist\mpl-data\images\stock_refresh.ppm
 File dist\mpl-data\images\stock_refresh.xpm
 File dist\mpl-data\images\stock_right.ppm
 File dist\mpl-data\images\stock_right.xpm
 File dist\mpl-data\images\stock_save_as.ppm
 File dist\mpl-data\images\stock_save_as.xpm
 File dist\mpl-data\images\stock_up.ppm
 File dist\mpl-data\images\stock_up.xpm
 File dist\mpl-data\images\stock_zoom-in.ppm
 File dist\mpl-data\images\stock_zoom-in.xpm
 File dist\mpl-data\images\stock_zoom-out.ppm
 File dist\mpl-data\images\stock_zoom-out.xpm
 File dist\mpl-data\images\subplots.png
 File dist\mpl-data\images\subplots.ppm
 File dist\mpl-data\images\subplots.xpm
 File dist\mpl-data\images\zoom_to_rect.png
 File dist\mpl-data\images\zoom_to_rect.ppm
 File dist\mpl-data\images\zoom_to_rect.svg
 File dist\mpl-data\images\zoom_to_rect.xpm


SetOutPath $INSTDIR\images
 File dist\images\puzzlebox.ico
 File dist\images\puzzlebox_logo.png


SetOutPath $INSTDIR\package
 File ..\packaging\win32\vcredist_x86.exe

ExecWait '"$INSTDIR\package\vcredist_x86.exe" /q:a /c:"msiexec /i vcredist.msi /qn /l*v %temp%\vcredist_x86.log"'


; write out uninstaller
WriteUninstaller "$INSTDIR\Puzzlebox-Synapse-Uninstall.exe"

SectionEnd ; end of default section


Section "Start Menu + Desktop Icons"
;  SetOutPath "$SMPROGRAMS\Puzzlebox Synapse"
  SetOutPath $INSTDIR
  CreateDirectory "$SMPROGRAMS\Puzzlebox Synapse"
  CreateShortCut "$SMPROGRAMS\Puzzlebox Synapse\Puzzlebox Synapse ThinkGear Connect Client.lnk" \
                 "$INSTDIR\synapse-client.exe" \
                 ""  "$INSTDIR\images\puzzlebox.ico"  "0"  ""
  CreateShortCut "$SMPROGRAMS\Puzzlebox Synapse\Puzzlebox Synapse.lnk" \
                 "$INSTDIR\synapse-gui.exe" \
                 ""  "$INSTDIR\images\puzzlebox.ico"  "0"  ""
  CreateShortCut "$SMPROGRAMS\Puzzlebox Synapse\Edit Puzzlebox Synapse Configuration.lnk" \
                 "$INSTDIR\puzzlebox_synapse_configuration.ini" \
                 ""  ""  "0"  ""
  CreateShortCut "$SMPROGRAMS\Puzzlebox Synapse\Uninstall Puzzlebox Synapse.lnk" \
                 "$INSTDIR\Puzzlebox-Synapse-Uninstall.exe"
  CreateShortCut "$DESKTOP\Puzzlebox Synapse.lnk" \
                 "$INSTDIR\synapse-gui.exe" \
                 ""  "$INSTDIR\images\puzzlebox.ico"  "0"  ""
SectionEnd


; begin uninstall settings/section
UninstallText "This will uninstall Puzzlebox Synapse from your system"

Section Uninstall
; add delete commands to delete whatever files/registry keys/etc you installed here.
 Delete "$SMPROGRAMS\Puzzlebox Synapse\Puzzlebox Synapse ThinkGear Connect Client.lnk"
 Delete "$SMPROGRAMS\Puzzlebox Synapse\Puzzlebox Synapse.lnk"
 Delete "$SMPROGRAMS\Puzzlebox Synapse\Edit Puzzlebox Synapse Configuration.lnk" 
 Delete "$SMPROGRAMS\Puzzlebox Synapse\Uninstall Puzzlebox Synapse.lnk" 
   RMDir "$SMPROGRAMS\Puzzlebox Synapse"
 Delete "$DESKTOP\Puzzlebox Synapse.lnk"


 Delete $INSTDIR\images\puzzlebox.ico
 Delete $INSTDIR\images\puzzlebox_logo.png
 Delete $INSTDIR\lib\library.zip
; Delete $INSTDIR\lib\tcl\tcl8.4\auto.tcl
; Delete $INSTDIR\lib\tcl\tcl8.4\encoding\ascii.enc
; Delete $INSTDIR\lib\tcl\tcl8.4\encoding\big5.enc
; Delete $INSTDIR\lib\tcl\tcl8.4\encoding\cp1250.enc
; Delete $INSTDIR\lib\tcl\tcl8.4\encoding\cp1251.enc
; Delete $INSTDIR\lib\tcl\tcl8.4\encoding\cp1252.enc
; Delete $INSTDIR\lib\tcl\tcl8.4\encoding\cp1253.enc
; Delete $INSTDIR\lib\tcl\tcl8.4\encoding\cp1254.enc
; Delete $INSTDIR\lib\tcl\tcl8.4\encoding\cp1255.enc
; Delete $INSTDIR\lib\tcl\tcl8.4\encoding\cp1256.enc
; Delete $INSTDIR\lib\tcl\tcl8.4\encoding\cp1257.enc
; Delete $INSTDIR\lib\tcl\tcl8.4\encoding\cp1258.enc
; Delete $INSTDIR\lib\tcl\tcl8.4\encoding\cp437.enc
; Delete $INSTDIR\lib\tcl\tcl8.4\encoding\cp737.enc
; Delete $INSTDIR\lib\tcl\tcl8.4\encoding\cp775.enc
; Delete $INSTDIR\lib\tcl\tcl8.4\encoding\cp850.enc
; Delete $INSTDIR\lib\tcl\tcl8.4\encoding\cp852.enc
; Delete $INSTDIR\lib\tcl\tcl8.4\encoding\cp855.enc
; Delete $INSTDIR\lib\tcl\tcl8.4\encoding\cp857.enc
; Delete $INSTDIR\lib\tcl\tcl8.4\encoding\cp860.enc
; Delete $INSTDIR\lib\tcl\tcl8.4\encoding\cp861.enc
; Delete $INSTDIR\lib\tcl\tcl8.4\encoding\cp862.enc
; Delete $INSTDIR\lib\tcl\tcl8.4\encoding\cp863.enc
; Delete $INSTDIR\lib\tcl\tcl8.4\encoding\cp864.enc
; Delete $INSTDIR\lib\tcl\tcl8.4\encoding\cp865.enc
; Delete $INSTDIR\lib\tcl\tcl8.4\encoding\cp866.enc
; Delete $INSTDIR\lib\tcl\tcl8.4\encoding\cp869.enc
; Delete $INSTDIR\lib\tcl\tcl8.4\encoding\cp874.enc
; Delete $INSTDIR\lib\tcl\tcl8.4\encoding\cp932.enc
; Delete $INSTDIR\lib\tcl\tcl8.4\encoding\cp936.enc
; Delete $INSTDIR\lib\tcl\tcl8.4\encoding\cp949.enc
; Delete $INSTDIR\lib\tcl\tcl8.4\encoding\cp950.enc
; Delete $INSTDIR\lib\tcl\tcl8.4\encoding\dingbats.enc
; Delete $INSTDIR\lib\tcl\tcl8.4\encoding\ebcdic.enc
; Delete $INSTDIR\lib\tcl\tcl8.4\encoding\euc-cn.enc
; Delete $INSTDIR\lib\tcl\tcl8.4\encoding\euc-jp.enc
; Delete $INSTDIR\lib\tcl\tcl8.4\encoding\euc-kr.enc
; Delete $INSTDIR\lib\tcl\tcl8.4\encoding\gb12345.enc
; Delete $INSTDIR\lib\tcl\tcl8.4\encoding\gb1988.enc
; Delete $INSTDIR\lib\tcl\tcl8.4\encoding\gb2312.enc
; Delete $INSTDIR\lib\tcl\tcl8.4\encoding\gb2312-raw.enc
; Delete $INSTDIR\lib\tcl\tcl8.4\encoding\iso2022.enc
; Delete $INSTDIR\lib\tcl\tcl8.4\encoding\iso2022-jp.enc
; Delete $INSTDIR\lib\tcl\tcl8.4\encoding\iso2022-kr.enc
; Delete $INSTDIR\lib\tcl\tcl8.4\encoding\iso8859-10.enc
; Delete $INSTDIR\lib\tcl\tcl8.4\encoding\iso8859-13.enc
; Delete $INSTDIR\lib\tcl\tcl8.4\encoding\iso8859-14.enc
; Delete $INSTDIR\lib\tcl\tcl8.4\encoding\iso8859-15.enc
; Delete $INSTDIR\lib\tcl\tcl8.4\encoding\iso8859-16.enc
; Delete $INSTDIR\lib\tcl\tcl8.4\encoding\iso8859-1.enc
; Delete $INSTDIR\lib\tcl\tcl8.4\encoding\iso8859-2.enc
; Delete $INSTDIR\lib\tcl\tcl8.4\encoding\iso8859-3.enc
; Delete $INSTDIR\lib\tcl\tcl8.4\encoding\iso8859-4.enc
; Delete $INSTDIR\lib\tcl\tcl8.4\encoding\iso8859-5.enc
; Delete $INSTDIR\lib\tcl\tcl8.4\encoding\iso8859-6.enc
; Delete $INSTDIR\lib\tcl\tcl8.4\encoding\iso8859-7.enc
; Delete $INSTDIR\lib\tcl\tcl8.4\encoding\iso8859-8.enc
; Delete $INSTDIR\lib\tcl\tcl8.4\encoding\iso8859-9.enc
; Delete $INSTDIR\lib\tcl\tcl8.4\encoding\jis0201.enc
; Delete $INSTDIR\lib\tcl\tcl8.4\encoding\jis0208.enc
; Delete $INSTDIR\lib\tcl\tcl8.4\encoding\jis0212.enc
; Delete $INSTDIR\lib\tcl\tcl8.4\encoding\koi8-r.enc
; Delete $INSTDIR\lib\tcl\tcl8.4\encoding\koi8-u.enc
; Delete $INSTDIR\lib\tcl\tcl8.4\encoding\ksc5601.enc
; Delete $INSTDIR\lib\tcl\tcl8.4\encoding\macCentEuro.enc
; Delete $INSTDIR\lib\tcl\tcl8.4\encoding\macCroatian.enc
; Delete $INSTDIR\lib\tcl\tcl8.4\encoding\macCyrillic.enc
; Delete $INSTDIR\lib\tcl\tcl8.4\encoding\macDingbats.enc
; Delete $INSTDIR\lib\tcl\tcl8.4\encoding\macGreek.enc
; Delete $INSTDIR\lib\tcl\tcl8.4\encoding\macIceland.enc
; Delete $INSTDIR\lib\tcl\tcl8.4\encoding\macJapan.enc
; Delete $INSTDIR\lib\tcl\tcl8.4\encoding\macRoman.enc
; Delete $INSTDIR\lib\tcl\tcl8.4\encoding\macRomania.enc
; Delete $INSTDIR\lib\tcl\tcl8.4\encoding\macThai.enc
; Delete $INSTDIR\lib\tcl\tcl8.4\encoding\macTurkish.enc
; Delete $INSTDIR\lib\tcl\tcl8.4\encoding\macUkraine.enc
; Delete $INSTDIR\lib\tcl\tcl8.4\encoding\shiftjis.enc
; Delete $INSTDIR\lib\tcl\tcl8.4\encoding\symbol.enc
; Delete $INSTDIR\lib\tcl\tcl8.4\encoding\tis-620.enc
; Delete $INSTDIR\lib\tcl\tcl8.4\history.tcl
; Delete $INSTDIR\lib\tcl\tcl8.4\http1.0\http.tcl
; Delete $INSTDIR\lib\tcl\tcl8.4\http1.0\pkgIndex.tcl
; Delete $INSTDIR\lib\tcl\tcl8.4\http2.5\http.tcl
; Delete $INSTDIR\lib\tcl\tcl8.4\http2.5\pkgIndex.tcl
; Delete $INSTDIR\lib\tcl\tcl8.4\init.tcl
; Delete $INSTDIR\lib\tcl\tcl8.4\ldAout.tcl
; Delete $INSTDIR\lib\tcl\tcl8.4\msgcat1.3\msgcat.tcl
; Delete $INSTDIR\lib\tcl\tcl8.4\msgcat1.3\pkgIndex.tcl
; Delete $INSTDIR\lib\tcl\tcl8.4\opt0.4\optparse.tcl
; Delete $INSTDIR\lib\tcl\tcl8.4\opt0.4\pkgIndex.tcl
; Delete $INSTDIR\lib\tcl\tcl8.4\package.tcl
; Delete $INSTDIR\lib\tcl\tcl8.4\parray.tcl
; Delete $INSTDIR\lib\tcl\tcl8.4\safe.tcl
; Delete $INSTDIR\lib\tcl\tcl8.4\tclIndex
; Delete $INSTDIR\lib\tcl\tcl8.4\tcltest2.2\pkgIndex.tcl
; Delete $INSTDIR\lib\tcl\tcl8.4\tcltest2.2\tcltest.tcl
; Delete $INSTDIR\lib\tcl\tcl8.4\word.tcl
; Delete $INSTDIR\lib\tcl\tk8.4\bgerror.tcl
; Delete $INSTDIR\lib\tcl\tk8.4\button.tcl
; Delete $INSTDIR\lib\tcl\tk8.4\choosedir.tcl
; Delete $INSTDIR\lib\tcl\tk8.4\clrpick.tcl
; Delete $INSTDIR\lib\tcl\tk8.4\comdlg.tcl
; Delete $INSTDIR\lib\tcl\tk8.4\console.tcl
; Delete $INSTDIR\lib\tcl\tk8.4\demos\arrow.tcl
; Delete $INSTDIR\lib\tcl\tk8.4\demos\bind.tcl
; Delete $INSTDIR\lib\tcl\tk8.4\demos\bitmap.tcl
; Delete $INSTDIR\lib\tcl\tk8.4\demos\browse
; Delete $INSTDIR\lib\tcl\tk8.4\demos\button.tcl
; Delete $INSTDIR\lib\tcl\tk8.4\demos\check.tcl
; Delete $INSTDIR\lib\tcl\tk8.4\demos\clrpick.tcl
; Delete $INSTDIR\lib\tcl\tk8.4\demos\colors.tcl
; Delete $INSTDIR\lib\tcl\tk8.4\demos\cscroll.tcl
; Delete $INSTDIR\lib\tcl\tk8.4\demos\ctext.tcl
; Delete $INSTDIR\lib\tcl\tk8.4\demos\dialog1.tcl
; Delete $INSTDIR\lib\tcl\tk8.4\demos\dialog2.tcl
; Delete $INSTDIR\lib\tcl\tk8.4\demos\entry1.tcl
; Delete $INSTDIR\lib\tcl\tk8.4\demos\entry2.tcl
; Delete $INSTDIR\lib\tcl\tk8.4\demos\entry3.tcl
; Delete $INSTDIR\lib\tcl\tk8.4\demos\filebox.tcl
; Delete $INSTDIR\lib\tcl\tk8.4\demos\floor.tcl
; Delete $INSTDIR\lib\tcl\tk8.4\demos\form.tcl
; Delete $INSTDIR\lib\tcl\tk8.4\demos\hello
; Delete $INSTDIR\lib\tcl\tk8.4\demos\hscale.tcl
; Delete $INSTDIR\lib\tcl\tk8.4\demos\icon.tcl
; Delete $INSTDIR\lib\tcl\tk8.4\demos\image1.tcl
; Delete $INSTDIR\lib\tcl\tk8.4\demos\image2.tcl
; Delete $INSTDIR\lib\tcl\tk8.4\demos\images\earth.gif
; Delete $INSTDIR\lib\tcl\tk8.4\demos\images\earthris.gif
; Delete $INSTDIR\lib\tcl\tk8.4\demos\images\face.bmp
; Delete $INSTDIR\lib\tcl\tk8.4\demos\images\flagdown.bmp
; Delete $INSTDIR\lib\tcl\tk8.4\demos\images\flagup.bmp
; Delete $INSTDIR\lib\tcl\tk8.4\demos\images\gray25.bmp
; Delete $INSTDIR\lib\tcl\tk8.4\demos\images\letters.bmp
; Delete $INSTDIR\lib\tcl\tk8.4\demos\images\noletter.bmp
; Delete $INSTDIR\lib\tcl\tk8.4\demos\images\pattern.bmp
; Delete $INSTDIR\lib\tcl\tk8.4\demos\images\tcllogo.gif
; Delete $INSTDIR\lib\tcl\tk8.4\demos\images\teapot.ppm
; Delete $INSTDIR\lib\tcl\tk8.4\demos\items.tcl
; Delete $INSTDIR\lib\tcl\tk8.4\demos\ixset
; Delete $INSTDIR\lib\tcl\tk8.4\demos\labelframe.tcl
; Delete $INSTDIR\lib\tcl\tk8.4\demos\label.tcl
; Delete $INSTDIR\lib\tcl\tk8.4\demos\license.terms
; Delete $INSTDIR\lib\tcl\tk8.4\demos\menubu.tcl
; Delete $INSTDIR\lib\tcl\tk8.4\demos\menu.tcl
; Delete $INSTDIR\lib\tcl\tk8.4\demos\msgbox.tcl
; Delete $INSTDIR\lib\tcl\tk8.4\demos\paned1.tcl
; Delete $INSTDIR\lib\tcl\tk8.4\demos\paned2.tcl
; Delete $INSTDIR\lib\tcl\tk8.4\demos\plot.tcl
; Delete $INSTDIR\lib\tcl\tk8.4\demos\puzzle.tcl
; Delete $INSTDIR\lib\tcl\tk8.4\demos\radio.tcl
; Delete $INSTDIR\lib\tcl\tk8.4\demos\README
; Delete $INSTDIR\lib\tcl\tk8.4\demos\rmt
; Delete $INSTDIR\lib\tcl\tk8.4\demos\rolodex
; Delete $INSTDIR\lib\tcl\tk8.4\demos\ruler.tcl
; Delete $INSTDIR\lib\tcl\tk8.4\demos\sayings.tcl
; Delete $INSTDIR\lib\tcl\tk8.4\demos\search.tcl
; Delete $INSTDIR\lib\tcl\tk8.4\demos\spin.tcl
; Delete $INSTDIR\lib\tcl\tk8.4\demos\square
; Delete $INSTDIR\lib\tcl\tk8.4\demos\states.tcl
; Delete $INSTDIR\lib\tcl\tk8.4\demos\style.tcl
; Delete $INSTDIR\lib\tcl\tk8.4\demos\tclIndex
; Delete $INSTDIR\lib\tcl\tk8.4\demos\tcolor
; Delete $INSTDIR\lib\tcl\tk8.4\demos\text.tcl
; Delete $INSTDIR\lib\tcl\tk8.4\demos\timer
; Delete $INSTDIR\lib\tcl\tk8.4\demos\twind.tcl
; Delete $INSTDIR\lib\tcl\tk8.4\demos\unicodeout.tcl
; Delete $INSTDIR\lib\tcl\tk8.4\demos\vscale.tcl
; Delete $INSTDIR\lib\tcl\tk8.4\demos\widget
; Delete $INSTDIR\lib\tcl\tk8.4\dialog.tcl
; Delete $INSTDIR\lib\tcl\tk8.4\entry.tcl
; Delete $INSTDIR\lib\tcl\tk8.4\focus.tcl
; Delete $INSTDIR\lib\tcl\tk8.4\images\logo100.gif
; Delete $INSTDIR\lib\tcl\tk8.4\images\logo64.gif
; Delete $INSTDIR\lib\tcl\tk8.4\images\logo.eps
; Delete $INSTDIR\lib\tcl\tk8.4\images\logoLarge.gif
; Delete $INSTDIR\lib\tcl\tk8.4\images\logoMed.gif
; Delete $INSTDIR\lib\tcl\tk8.4\images\pwrdLogo100.gif
; Delete $INSTDIR\lib\tcl\tk8.4\images\pwrdLogo150.gif
; Delete $INSTDIR\lib\tcl\tk8.4\images\pwrdLogo175.gif
; Delete $INSTDIR\lib\tcl\tk8.4\images\pwrdLogo200.gif
; Delete $INSTDIR\lib\tcl\tk8.4\images\pwrdLogo75.gif
; Delete $INSTDIR\lib\tcl\tk8.4\images\pwrdLogo.eps
; Delete $INSTDIR\lib\tcl\tk8.4\images\README
; Delete $INSTDIR\lib\tcl\tk8.4\images\tai-ku.gif
; Delete $INSTDIR\lib\tcl\tk8.4\license.terms
; Delete $INSTDIR\lib\tcl\tk8.4\listbox.tcl
; Delete $INSTDIR\lib\tcl\tk8.4\menu.tcl
; Delete $INSTDIR\lib\tcl\tk8.4\mkpsenc.tcl
; Delete $INSTDIR\lib\tcl\tk8.4\msgbox.tcl
; Delete $INSTDIR\lib\tcl\tk8.4\msgs\cs.msg
; Delete $INSTDIR\lib\tcl\tk8.4\msgs\de.msg
; Delete $INSTDIR\lib\tcl\tk8.4\msgs\el.msg
; Delete $INSTDIR\lib\tcl\tk8.4\msgs\en_gb.msg
; Delete $INSTDIR\lib\tcl\tk8.4\msgs\en.msg
; Delete $INSTDIR\lib\tcl\tk8.4\msgs\eo.msg
; Delete $INSTDIR\lib\tcl\tk8.4\msgs\es_ES.msg
; Delete $INSTDIR\lib\tcl\tk8.4\msgs\es.msg
; Delete $INSTDIR\lib\tcl\tk8.4\msgs\fr.msg
; Delete $INSTDIR\lib\tcl\tk8.4\msgs\it.msg
; Delete $INSTDIR\lib\tcl\tk8.4\msgs\nl.msg
; Delete $INSTDIR\lib\tcl\tk8.4\msgs\pl.msg
; Delete $INSTDIR\lib\tcl\tk8.4\msgs\ru.msg
; Delete $INSTDIR\lib\tcl\tk8.4\obsolete.tcl
; Delete $INSTDIR\lib\tcl\tk8.4\optMenu.tcl
; Delete $INSTDIR\lib\tcl\tk8.4\palette.tcl
; Delete $INSTDIR\lib\tcl\tk8.4\panedwindow.tcl
; Delete $INSTDIR\lib\tcl\tk8.4\pkgIndex.tcl
; Delete $INSTDIR\lib\tcl\tk8.4\safetk.tcl
; Delete $INSTDIR\lib\tcl\tk8.4\scale.tcl
; Delete $INSTDIR\lib\tcl\tk8.4\scrlbar.tcl
; Delete $INSTDIR\lib\tcl\tk8.4\spinbox.tcl
; Delete $INSTDIR\lib\tcl\tk8.4\tclIndex
; Delete $INSTDIR\lib\tcl\tk8.4\tearoff.tcl
; Delete $INSTDIR\lib\tcl\tk8.4\text.tcl
; Delete $INSTDIR\lib\tcl\tk8.4\tkfbox.tcl
; Delete $INSTDIR\lib\tcl\tk8.4\tk.tcl
; Delete $INSTDIR\lib\tcl\tk8.4\unsupported.tcl
; Delete $INSTDIR\lib\tcl\tk8.4\xmfbox.tcl
 Delete $INSTDIR\mpl-data\fonts\afm\cmex10.afm
 Delete $INSTDIR\mpl-data\fonts\afm\cmmi10.afm
 Delete $INSTDIR\mpl-data\fonts\afm\cmr10.afm
 Delete $INSTDIR\mpl-data\fonts\afm\cmsy10.afm
 Delete $INSTDIR\mpl-data\fonts\afm\cmtt10.afm
 Delete $INSTDIR\mpl-data\fonts\afm\pagd8a.afm
 Delete $INSTDIR\mpl-data\fonts\afm\pagdo8a.afm
 Delete $INSTDIR\mpl-data\fonts\afm\pagk8a.afm
 Delete $INSTDIR\mpl-data\fonts\afm\pagko8a.afm
 Delete $INSTDIR\mpl-data\fonts\afm\pbkd8a.afm
 Delete $INSTDIR\mpl-data\fonts\afm\pbkdi8a.afm
 Delete $INSTDIR\mpl-data\fonts\afm\pbkl8a.afm
 Delete $INSTDIR\mpl-data\fonts\afm\pbkli8a.afm
 Delete $INSTDIR\mpl-data\fonts\afm\pcrb8a.afm
 Delete $INSTDIR\mpl-data\fonts\afm\pcrbo8a.afm
 Delete $INSTDIR\mpl-data\fonts\afm\pcrr8a.afm
 Delete $INSTDIR\mpl-data\fonts\afm\pcrro8a.afm
 Delete $INSTDIR\mpl-data\fonts\afm\phvb8a.afm
 Delete $INSTDIR\mpl-data\fonts\afm\phvb8an.afm
 Delete $INSTDIR\mpl-data\fonts\afm\phvbo8a.afm
 Delete $INSTDIR\mpl-data\fonts\afm\phvbo8an.afm
 Delete $INSTDIR\mpl-data\fonts\afm\phvl8a.afm
 Delete $INSTDIR\mpl-data\fonts\afm\phvlo8a.afm
 Delete $INSTDIR\mpl-data\fonts\afm\phvr8a.afm
 Delete $INSTDIR\mpl-data\fonts\afm\phvr8an.afm
 Delete $INSTDIR\mpl-data\fonts\afm\phvro8a.afm
 Delete $INSTDIR\mpl-data\fonts\afm\phvro8an.afm
 Delete $INSTDIR\mpl-data\fonts\afm\pncb8a.afm
 Delete $INSTDIR\mpl-data\fonts\afm\pncbi8a.afm
 Delete $INSTDIR\mpl-data\fonts\afm\pncr8a.afm
 Delete $INSTDIR\mpl-data\fonts\afm\pncri8a.afm
 Delete $INSTDIR\mpl-data\fonts\afm\pplb8a.afm
 Delete $INSTDIR\mpl-data\fonts\afm\pplbi8a.afm
 Delete $INSTDIR\mpl-data\fonts\afm\pplr8a.afm
 Delete $INSTDIR\mpl-data\fonts\afm\pplri8a.afm
 Delete $INSTDIR\mpl-data\fonts\afm\psyr.afm
 Delete $INSTDIR\mpl-data\fonts\afm\ptmb8a.afm
 Delete $INSTDIR\mpl-data\fonts\afm\ptmbi8a.afm
 Delete $INSTDIR\mpl-data\fonts\afm\ptmr8a.afm
 Delete $INSTDIR\mpl-data\fonts\afm\ptmri8a.afm
 Delete $INSTDIR\mpl-data\fonts\afm\putb8a.afm
 Delete $INSTDIR\mpl-data\fonts\afm\putbi8a.afm
 Delete $INSTDIR\mpl-data\fonts\afm\putr8a.afm
 Delete $INSTDIR\mpl-data\fonts\afm\putri8a.afm
 Delete $INSTDIR\mpl-data\fonts\afm\pzcmi8a.afm
 Delete $INSTDIR\mpl-data\fonts\afm\pzdr.afm
 Delete $INSTDIR\mpl-data\fonts\pdfcorefonts\Courier.afm
 Delete $INSTDIR\mpl-data\fonts\pdfcorefonts\Courier-Bold.afm
 Delete $INSTDIR\mpl-data\fonts\pdfcorefonts\Courier-BoldOblique.afm
 Delete $INSTDIR\mpl-data\fonts\pdfcorefonts\Courier-Oblique.afm
 Delete $INSTDIR\mpl-data\fonts\pdfcorefonts\Helvetica.afm
 Delete $INSTDIR\mpl-data\fonts\pdfcorefonts\Helvetica-Bold.afm
 Delete $INSTDIR\mpl-data\fonts\pdfcorefonts\Helvetica-BoldOblique.afm
 Delete $INSTDIR\mpl-data\fonts\pdfcorefonts\Helvetica-Oblique.afm
 Delete $INSTDIR\mpl-data\fonts\pdfcorefonts\readme.txt
 Delete $INSTDIR\mpl-data\fonts\pdfcorefonts\Symbol.afm
 Delete $INSTDIR\mpl-data\fonts\pdfcorefonts\Times-Bold.afm
 Delete $INSTDIR\mpl-data\fonts\pdfcorefonts\Times-BoldItalic.afm
 Delete $INSTDIR\mpl-data\fonts\pdfcorefonts\Times-Italic.afm
 Delete $INSTDIR\mpl-data\fonts\pdfcorefonts\Times-Roman.afm
 Delete $INSTDIR\mpl-data\fonts\pdfcorefonts\ZapfDingbats.afm
 Delete $INSTDIR\mpl-data\fonts\ttf\cmb10.ttf
 Delete $INSTDIR\mpl-data\fonts\ttf\cmex10.ttf
 Delete $INSTDIR\mpl-data\fonts\ttf\cmmi10.ttf
 Delete $INSTDIR\mpl-data\fonts\ttf\cmr10.ttf
 Delete $INSTDIR\mpl-data\fonts\ttf\cmss10.ttf
 Delete $INSTDIR\mpl-data\fonts\ttf\cmsy10.ttf
 Delete $INSTDIR\mpl-data\fonts\ttf\cmtt10.ttf
 Delete $INSTDIR\mpl-data\fonts\ttf\COPYRIGHT.TXT
 Delete $INSTDIR\mpl-data\fonts\ttf\LICENSE_STIX
 Delete $INSTDIR\mpl-data\fonts\ttf\README.TXT
 Delete $INSTDIR\mpl-data\fonts\ttf\RELEASENOTES.TXT
 Delete $INSTDIR\mpl-data\fonts\ttf\STIXGeneralBolIta.ttf
 Delete $INSTDIR\mpl-data\fonts\ttf\STIXGeneralBol.ttf
 Delete $INSTDIR\mpl-data\fonts\ttf\STIXGeneralItalic.ttf
 Delete $INSTDIR\mpl-data\fonts\ttf\STIXGeneral.ttf
 Delete $INSTDIR\mpl-data\fonts\ttf\STIXNonUniBolIta.ttf
 Delete $INSTDIR\mpl-data\fonts\ttf\STIXNonUniBol.ttf
 Delete $INSTDIR\mpl-data\fonts\ttf\STIXNonUniIta.ttf
 Delete $INSTDIR\mpl-data\fonts\ttf\STIXNonUni.ttf
 Delete $INSTDIR\mpl-data\fonts\ttf\STIXSizFiveSymReg.ttf
 Delete $INSTDIR\mpl-data\fonts\ttf\STIXSizFourSymBol.ttf
 Delete $INSTDIR\mpl-data\fonts\ttf\STIXSizFourSymReg.ttf
 Delete $INSTDIR\mpl-data\fonts\ttf\STIXSizOneSymBol.ttf
 Delete $INSTDIR\mpl-data\fonts\ttf\STIXSizOneSymReg.ttf
 Delete $INSTDIR\mpl-data\fonts\ttf\STIXSizThreeSymBol.ttf
 Delete $INSTDIR\mpl-data\fonts\ttf\STIXSizThreeSymReg.ttf
 Delete $INSTDIR\mpl-data\fonts\ttf\STIXSizTwoSymBol.ttf
 Delete $INSTDIR\mpl-data\fonts\ttf\STIXSizTwoSymReg.ttf
 Delete $INSTDIR\mpl-data\fonts\ttf\VeraBd.ttf
 Delete $INSTDIR\mpl-data\fonts\ttf\VeraBI.ttf
 Delete $INSTDIR\mpl-data\fonts\ttf\VeraIt.ttf
 Delete $INSTDIR\mpl-data\fonts\ttf\VeraMoBd.ttf
 Delete $INSTDIR\mpl-data\fonts\ttf\VeraMoBI.ttf
 Delete $INSTDIR\mpl-data\fonts\ttf\VeraMoIt.ttf
 Delete $INSTDIR\mpl-data\fonts\ttf\VeraMono.ttf
 Delete $INSTDIR\mpl-data\fonts\ttf\VeraSeBd.ttf
 Delete $INSTDIR\mpl-data\fonts\ttf\VeraSe.ttf
 Delete $INSTDIR\mpl-data\fonts\ttf\Vera.ttf
 Delete $INSTDIR\mpl-data\images\back.png
 Delete $INSTDIR\mpl-data\images\back.ppm
 Delete $INSTDIR\mpl-data\images\back.svg
 Delete $INSTDIR\mpl-data\images\back.xpm
 Delete $INSTDIR\mpl-data\images\filesave.png
 Delete $INSTDIR\mpl-data\images\filesave.ppm
 Delete $INSTDIR\mpl-data\images\filesave.svg
 Delete $INSTDIR\mpl-data\images\filesave.xpm
 Delete $INSTDIR\mpl-data\images\forward.png
 Delete $INSTDIR\mpl-data\images\forward.ppm
 Delete $INSTDIR\mpl-data\images\forward.svg
 Delete $INSTDIR\mpl-data\images\forward.xpm
 Delete $INSTDIR\mpl-data\images\hand.png
 Delete $INSTDIR\mpl-data\images\hand.ppm
 Delete $INSTDIR\mpl-data\images\hand.svg
 Delete $INSTDIR\mpl-data\images\hand.xpm
 Delete $INSTDIR\mpl-data\images\home.png
 Delete $INSTDIR\mpl-data\images\home.ppm
 Delete $INSTDIR\mpl-data\images\home.svg
 Delete $INSTDIR\mpl-data\images\home.xpm
 Delete $INSTDIR\mpl-data\images\matplotlib.png
 Delete $INSTDIR\mpl-data\images\matplotlib.svg
 Delete $INSTDIR\mpl-data\images\move.png
 Delete $INSTDIR\mpl-data\images\move.ppm
 Delete $INSTDIR\mpl-data\images\move.svg
 Delete $INSTDIR\mpl-data\images\move.xpm
 Delete $INSTDIR\mpl-data\images\qt4_editor_options.svg
 Delete $INSTDIR\mpl-data\images\stock_close.ppm
 Delete $INSTDIR\mpl-data\images\stock_close.xpm
 Delete $INSTDIR\mpl-data\images\stock_down.ppm
 Delete $INSTDIR\mpl-data\images\stock_down.xpm
 Delete $INSTDIR\mpl-data\images\stock_left.ppm
 Delete $INSTDIR\mpl-data\images\stock_left.xpm
 Delete $INSTDIR\mpl-data\images\stock_refresh.ppm
 Delete $INSTDIR\mpl-data\images\stock_refresh.xpm
 Delete $INSTDIR\mpl-data\images\stock_right.ppm
 Delete $INSTDIR\mpl-data\images\stock_right.xpm
 Delete $INSTDIR\mpl-data\images\stock_save_as.ppm
 Delete $INSTDIR\mpl-data\images\stock_save_as.xpm
 Delete $INSTDIR\mpl-data\images\stock_up.ppm
 Delete $INSTDIR\mpl-data\images\stock_up.xpm
 Delete $INSTDIR\mpl-data\images\stock_zoom-in.ppm
 Delete $INSTDIR\mpl-data\images\stock_zoom-in.xpm
 Delete $INSTDIR\mpl-data\images\stock_zoom-out.ppm
 Delete $INSTDIR\mpl-data\images\stock_zoom-out.xpm
 Delete $INSTDIR\mpl-data\images\subplots.png
 Delete $INSTDIR\mpl-data\images\subplots.ppm
 Delete $INSTDIR\mpl-data\images\subplots.xpm
 Delete $INSTDIR\mpl-data\images\zoom_to_rect.png
 Delete $INSTDIR\mpl-data\images\zoom_to_rect.ppm
 Delete $INSTDIR\mpl-data\images\zoom_to_rect.svg
 Delete $INSTDIR\mpl-data\images\zoom_to_rect.xpm
 Delete $INSTDIR\mpl-data\lineprops.glade
 Delete $INSTDIR\mpl-data\matplotlib.conf
 Delete $INSTDIR\mpl-data\matplotlibrc
 Delete $INSTDIR\msvcr71.dll
 Delete $INSTDIR\puzzlebox_synapse_configuration.ini
 Delete $INSTDIR\python25.dll
 Delete $INSTDIR\synapse-client.exe
 Delete $INSTDIR\synapse-gui.exe
 Delete $INSTDIR\synapse-protocol.exe
 Delete $INSTDIR\synapse-server.exe
 Delete $INSTDIR\w9xpopen.exe

 RMDir $INSTDIR\images
 RMDir $INSTDIR\lib
 RMDir $INSTDIR\mpl-data\fonts\afm
 RMDir $INSTDIR\mpl-data\fonts\pdfcorefonts
 RMDir $INSTDIR\mpl-data\fonts\ttf
 RMDir $INSTDIR\mpl-data\fonts
 RMDir $INSTDIR\mpl-data\images
 RMDir $INSTDIR\mpl-data


 Delete $INSTDIR\package\vcredist_x86.exe
 RMDIR $INSTDIR\package


Delete "$INSTDIR\Puzzlebox-Synapse-Uninstall.exe"
DeleteRegKey HKEY_LOCAL_MACHINE "SOFTWARE\Puzzlebox Synapse\Puzzlebox Synapse"
DeleteRegKey HKEY_LOCAL_MACHINE "SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\Puzzlebox Synapse"


 RMDIR $INSTDIR


  ; if $INSTDIR was removed, skip these next ones
  IfFileExists $INSTDIR 0 Removed 
    MessageBox MB_YESNO|MB_ICONQUESTION \
      "Remove all files in your Puzzlebox Synapse directory? (If you have anything \
you created that you want to keep, click No)" IDNO Removed
    Delete $INSTDIR\*.* ; this would be skipped if the user hits no
    RMDir /r $INSTDIR
    IfFileExists $INSTDIR 0 Removed 
      MessageBox MB_OK|MB_ICONEXCLAMATION \
                 "Note: $INSTDIR could not be removed."
  Removed:


SectionEnd ; end of uninstall section

; eof

