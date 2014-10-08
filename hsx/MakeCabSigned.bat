rem
rem $Id$
rem
rem $Log$
rem Revision 1.6  2009-01-02 18:19:09  bergsma
rem Info tag now says securebob.com, not abinition.com
rem
rem Revision 1.5  2008-10-25 22:14:02  bergsma
rem Correct Common Name
rem
rem Revision 1.4  2008-06-06 00:48:36  bergsma
rem no message
rem
rem Revision 1.3  2006-08-29 01:15:04  bergsma
rem no message
rem
rem Revision 1.2  2005/02/11 19:18:26  jbergsma
rem Added a /s option to perform only the signing step.
rem
rem Revision 1.1  2005/02/06 18:19:40  jbergsma
rem MS-DOS script to create and digitally sign the WebPickle CAB file.
rem
rem
rem Script to build and digitally sign the WebPickle CAB file for Internet Component Download.
rem
rem Usage: MakeCabSigned [/s]
rem
rem        where /s means only perform the signing step after building the CAB file
rem        because the certificate exists from running MakeCabSigned previously.
rem

echo off

rem Create the CAB file.
rem NOTE: makecab comes with Windows 2000+

makecab release\hsx.dll WebPickle.cab

rem If the /s option is specified then only perform the signing step

rem if %1 == /s goto SIGNIT

rem The certificate is created using MicroSoft Authenticode for Internet Explorer 5.0.
rem Download codesigningx86.exe from www.microsoft.com.
rem 
rem Create a self-signed X.509 certificate named myNewRoot using the default test root.
rem Also create a .pvk (private key) file and output the certificate to both My store and a file.
rem
rem Note: My store is located in:
rem C:\Documents and Settings\<USERNAME>\Application\Data\Microsoft\SystemCertificates\My\Certificates
rem
rem You can use CertMgr to view certificates (created in Personal certificates).

MakeCert -sv MyNewRootKey.pvk -r -n "CN=abinition.com, OU=Abinition, SN=Bergsma, E=bergsma@abinition.com" -ss My MyNewRoot.cer

rem Now create the SPC file. (A PKCS #7 signed-data object.)

Cert2SPC MyNewRoot.cer MyNewRoot.spc

rem Finally, sign the CAB file with the newly created certificate.

rem :SIGNIT

SignCode -spc MyNewRoot.spc -v MyNewRootKey.pvk -n "WebPickle Control" -i http://securebob.com -t http://timestamp.verisign.com/scripts/timstamp.dll WebPickle.cab
rem
rem SignCode -spc MyNewRoot.spc -v MyNewRootKey.pvk -n "WebPickle Control" -i http://www.abinition.com WebPickle.cab
rem
rem Copy the Webpickle.cab into the deploy directory
rem
copy Webpickle.cab ..\deploy
rem
