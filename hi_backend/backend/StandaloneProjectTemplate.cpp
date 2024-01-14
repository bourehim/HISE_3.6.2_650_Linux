/*  ===========================================================================
 *
 *   This file is part of HISE.
 *   Copyright 2016 Christoph Hart
 *
 *   HISE is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   HISE is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with HISE.  If not, see <http://www.gnu.org/licenses/>.
 *
 *   Commercial licenses for using HISE in an closed source project are
 *   available on request. Please visit the project's website to get more
 *   information about commercial licensing:
 *
 *   http://www.hise.audio/
 *
 *   HISE is based on the JUCE library,
 *   which must be separately licensed for closed source applications:
 *
 *   http://www.juce.com
 *
 *   ===========================================================================
 */

namespace hise {
using namespace juce;

// This is a autogenerated file that contains the template for the plugin exporter .jucer file

static const unsigned char projectStandaloneTemplate_jucer_lines[] =
"<?xml version=\"1.0\" encoding=\"UTF-8\"?>\r\n"
"\r\n"
"<JUCERPROJECT id=\"Tw64Zd\" name=\"%NAME%\" projectType=\"guiapp\" version=\"%VERSION%\"\r\n"
"              bundleIdentifier=\"%BUNDLE_ID%\" includeBinaryInAppConfig=\"1\"\r\n"
"              jucerVersion=\"5.2.0\" companyName=\"%COMPANY%\" companyWebsite=\"%COMPANY_WEBSITE%\" companyCopyright=\"%COMPANY_COPYRIGHT%\" displaySplashScreen=\"0\" reportAppUsage=\"0\" cppLanguageStandard=\"17\">\r\n"
"  <MAINGROUP id=\"SLR7uY\" name=\"%NAME%\">\r\n"
"    <GROUP id=\"{122C85F1-8B09-257A-B636-113E3EAC258A}\" name=\"Source\">\r\n"
"      <FILE id=\"eLP6Ii\" name=\"balanceKnob_200.png\" compile=\"0\" resource=\"1\"\r\n"
"            file=\"Source/Images/balanceKnob_200.png\"/>\r\n"
"      <FILE id=\"O9wjCd\" name=\"FrontendKnob_Bipolar.png\" compile=\"0\" resource=\"1\"\r\n"
"            file=\"Source/Images/FrontendKnob_Bipolar.png\"/>\r\n"
"      <FILE id=\"UAxUWd\" name=\"FrontendKnob_Unipolar.png\" compile=\"0\" resource=\"1\"\r\n"
"            file=\"Source/Images/FrontendKnob_Unipolar.png\"/>\r\n"
"      <FILE id=\"KmyOlv\" name=\"infoError.png\" compile=\"0\" resource=\"1\" file=\"Source/Images/infoError.png\"/>\r\n"
"      <FILE id=\"CNpA5r\" name=\"infoInfo.png\" compile=\"0\" resource=\"1\" file=\"Source/Images/infoInfo.png\"/>\r\n"
"      <FILE id=\"MjbOy3\" name=\"infoQuestion.png\" compile=\"0\" resource=\"1\"\r\n"
"            file=\"Source/Images/infoQuestion.png\"/>\r\n"
"      <FILE id=\"zsTpOz\" name=\"infoWarning.png\" compile=\"0\" resource=\"1\" file=\"Source/Images/infoWarning.png\"/>\r\n"
"      <FILE id=\"d1rhrw\" name=\"knobEmpty_200.png\" compile=\"0\" resource=\"1\"\r\n"
"            file=\"Source/Images/knobEmpty_200.png\"/>\r\n"
"      <FILE id=\"u2SLs3\" name=\"knobModulated_200.png\" compile=\"0\" resource=\"1\"\r\n"
"            file=\"Source/Images/knobModulated_200.png\"/>\r\n"
"      <FILE id=\"rTpn8D\" name=\"knobUnmodulated_200.png\" compile=\"0\" resource=\"1\"\r\n"
"            file=\"Source/Images/knobUnmodulated_200.png\"/>\r\n"
"      <FILE id=\"AOT2K8\" name=\"Plugin.cpp\" compile=\"1\" resource=\"0\" file=\"Source/Plugin.cpp\"/>\r\n"
"      <FILE id=\"jJzDA2\" name=\"CopyProtection.cpp\" compile=\"1\" resource=\"0\" file=\"Source/CopyProtection.cpp\"/>\r\n"
"      <FILE id=\"esGQuC\" name=\"PresetData.cpp\" compile=\"1\" resource=\"0\" file=\"Source/PresetData.cpp\"/>\r\n"
"      <FILE id=\"q8WZ82\" name=\"PresetData.h\" compile=\"0\" resource=\"0\" file=\"Source/PresetData.h\"/>\r\n"
"      <FILE id=\"jjzOA2\" name=\"toggle_200.png\" compile=\"0\" resource=\"1\" file=\"Source/Images/toggle_200.png\"/>\r\n"
"      %ADDITIONAL_FILES%\r\n"
"    </GROUP>\r\n"
"  </MAINGROUP>\r\n"
"  <EXPORTFORMATS>\r\n"
"    <%VS_VERSION% targetFolder=\"Builds/%TARGET_FOLDER%/\" IPP1ALibrary=\"%IPP_1A%\"  %ICON_FILE% extraDefs=\"%EXTRA_DEFINES_WIN%\"  extraLinkerFlags=\"%UAC_LEVEL%\" extraCompilerFlags=\"/bigobj /cgthreads8 %MSVC_WARNINGS%\">\r\n"
"      <CONFIGURATIONS>\r\n"
"        <CONFIGURATION name=\"Debug\" winWarningLevel=\"1\" generateManifest=\"1\" winArchitecture=\"x64\"\r\n"
"                       libraryPath=\"%WIN_STATIC_LIB_FOLDER_D64%\" isDebug=\"1\" optimisation=\"1\" targetName=\"%NAME% Debug\"\r\n"
"                       binaryPath=\"Compiled/\" headerPath=\"%ASIO_SDK_PATH%;%FAUST_HEADER_PATH%\" useRuntimeLibDLL=\"0\" prebuildCommand=\"%PREBUILD_COMMAND%\"/>\r\n"
"        <CONFIGURATION name=\"Release\" winWarningLevel=\"1\" generateManifest=\"1\" winArchitecture=\"x64\"\r\n"
"                       libraryPath=\"%WIN_STATIC_LIB_FOLDER_R64%\" isDebug=\"0\" optimisation=\"3\" targetName=\"%NAME%\"\r\n"
"                       binaryPath=\"Compiled/\" headerPath=\"%ASIO_SDK_PATH%;%FAUST_HEADER_PATH%\" linkTimeOptimisation=\"%LINK_TIME_OPTIMISATION%\" useRuntimeLibDLL=\"0\" prebuildCommand=\"%PREBUILD_COMMAND%\"/>\r\n"
"      </CONFIGURATIONS>\r\n"
"      <MODULEPATHS>\r\n"
"        <MODULEPATH id=\"juce_core\" path=\"%JUCE_PATH%\"/>\r\n"
"        <MODULEPATH id=\"juce_events\" path=\"%JUCE_PATH%\"/>\r\n"
"        <MODULEPATH id=\"juce_graphics\" path=\"%JUCE_PATH%\"/>\r\n"
"        <MODULEPATH id=\"juce_data_structures\" path=\"%JUCE_PATH%\"/>\r\n"
"        <MODULEPATH id=\"juce_dsp\" path=\"%JUCE_PATH%\"/>\r\n"
"        <MODULEPATH id=\"juce_gui_basics\" path=\"%JUCE_PATH%\"/>\r\n"
"        <MODULEPATH id=\"juce_gui_extra\" path=\"%JUCE_PATH%\"/>\r\n"
"        <MODULEPATH id=\"juce_cryptography\" path=\"%JUCE_PATH%\"/>\r\n"
"        <MODULEPATH id=\"juce_audio_basics\" path=\"%JUCE_PATH%\"/>\r\n"
"        <MODULEPATH id=\"juce_audio_devices\" path=\"%JUCE_PATH%\"/>\r\n"
"        <MODULEPATH id=\"juce_audio_formats\" path=\"%JUCE_PATH%\"/>\r\n"
"        <MODULEPATH id=\"juce_audio_processors\" path=\"%JUCE_PATH%\"/>\r\n"
"        <MODULEPATH id=\"juce_product_unlocking\" path=\"%JUCE_PATH%\"/>\r\n"
"        <MODULEPATH id=\"juce_audio_utils\" path=\"%JUCE_PATH%\"/>\r\n"
"        <MODULEPATH id=\"juce_opengl\" path=\"%JUCE_PATH%\"/>\r\n"
"        <MODULEPATH id=\"juce_osc\" path=\"%JUCE_PATH%\"/>\r\n"
"        <MODULEPATH id=\"hi_dsp_library\" path=\"%HISE_PATH%\"/>\r\n"
"        <MODULEPATH id=\"hi_faust\" path=\"%HISE_PATH%\"/>\r\n"
"        <MODULEPATH id=\"hi_faust_types\" path=\"%HISE_PATH%\"/>\r\n"
"        <MODULEPATH id=\"hi_frontend\" path=\"%HISE_PATH%\"/>\r\n"
"        <MODULEPATH id=\"hi_scripting\" path=\"%HISE_PATH%\"/>\r\n"
"        <MODULEPATH id=\"hi_core\" path=\"%HISE_PATH%\"/>\r\n"
"        <MODULEPATH id=\"hi_lac\" path=\"%HISE_PATH%\"/>\r\n"
"        <MODULEPATH id=\"hi_zstd\" path=\"%HISE_PATH%\"/>\r\n"
"        <MODULEPATH id=\"hi_rlottie\" path=\"%HISE_PATH%\"/>\r\n"
"        <MODULEPATH id=\"hi_streaming\" path=\"%HISE_PATH%\"/>\r\n"
"        <MODULEPATH id=\"hi_tools\" path=\"%HISE_PATH%\"/>\r\n"
"        <MODULEPATH id=\"hi_snex\" path=\"%HISE_PATH%\"/>\r\n"
"      </MODULEPATHS>\r\n"
"    </%VS_VERSION%>\r\n"
"    <XCODE_MAC targetFolder=\"Builds/MacOSX\" vstFolder=\"%VSTSDK_FOLDER%\" extraCompilerFlags=\"-Wno-reorder -Wno-inconsistent-missing-override -fno-aligned-allocation\" %ICON_FILE% \r\n"
"               extraLinkerFlags=\"%IPP_COMPILER_FLAGS% %OSX_STATIC_LIBS%\" extraDefs=\"%EXTRA_DEFINES_OSX%\" hardenedRuntime=\"0\" hardenedRuntimeOptions=\"com.apple.security.cs.allow-jit,com.apple.security.cs.allow-unsigned-executable-memory,com.apple.security.device.audio-input\" xcodeValidArchs=\"arm64,arm64e,x86_64\" externalLibraries=\"%BEATPORT_LIB_MACOS%\">\r\n"
"      <CONFIGURATIONS>\r\n"
"        <CONFIGURATION name=\"Debug\" osxSDK=\"default\" osxCompatibility=\"10.9 SDK\" osxArchitecture=\"%MACOS_ARCHITECTURE%\"\r\n"
"                       isDebug=\"1\" optimisation=\"1\" targetName=\"%NAME% Debug\"\r\n"
"                       headerPath=\"%IPP_HEADER%;%FAUST_HEADER_PATH%\" libraryPath=\"%IPP_LIBRARY%;%BEATPORT_DEBUG_LIB%\"\r\n"
"                       cppLibType=\"libc++\" binaryPath=\"Compiled/\"/>\r\n"
"        <CONFIGURATION name=\"Release\" osxSDK=\"default\" osxCompatibility=\"10.9 SDK\" osxArchitecture=\"%MACOS_ARCHITECTURE%\"\r\n"
"                       isDebug=\"0\" optimisation=\"3\" targetName=\"%NAME%\" headerPath=\"%IPP_HEADER%;%FAUST_HEADER_PATH%\"\r\n"
"                       libraryPath=\"%IPP_LIBRARY%;%BEATPORT_RELEASE_LIB%\" cppLibType=\"libc++\" linkTimeOptimisation=\"%LINK_TIME_OPTIMISATION%\"\r\n"
"                       binaryPath=\"Compiled/\" stripLocalSymbols=\"1\"/>\r\n"
"      </CONFIGURATIONS>\r\n"
"      <MODULEPATHS>\r\n"
"        <MODULEPATH id=\"juce_product_unlocking\" path=\"%JUCE_PATH%\"/>\r\n"
"        <MODULEPATH id=\"juce_gui_extra\" path=\"%JUCE_PATH%\"/>\r\n"
"        <MODULEPATH id=\"juce_gui_basics\" path=\"%JUCE_PATH%\"/>\r\n"
"        <MODULEPATH id=\"juce_graphics\" path=\"%JUCE_PATH%\"/>\r\n"
"        <MODULEPATH id=\"juce_events\" path=\"%JUCE_PATH%\"/>\r\n"
"        <MODULEPATH id=\"juce_data_structures\" path=\"%JUCE_PATH%\"/>\r\n"
"        <MODULEPATH id=\"juce_dsp\" path=\"%JUCE_PATH%\"/>\r\n"
"        <MODULEPATH id=\"juce_cryptography\" path=\"%JUCE_PATH%\"/>\r\n"
"        <MODULEPATH id=\"juce_core\" path=\"%JUCE_PATH%\"/>\r\n"
"        <MODULEPATH id=\"juce_audio_utils\" path=\"%JUCE_PATH%\"/>\r\n"
"        <MODULEPATH id=\"juce_audio_processors\" path=\"%JUCE_PATH%\"/>\r\n"
"        <MODULEPATH id=\"juce_audio_formats\" path=\"%JUCE_PATH%\"/>\r\n"
"        <MODULEPATH id=\"juce_audio_devices\" path=\"%JUCE_PATH%\"/>\r\n"
"        <MODULEPATH id=\"juce_audio_basics\" path=\"%JUCE_PATH%\"/>\r\n"
"        <MODULEPATH id=\"juce_opengl\" path=\"%JUCE_PATH%\"/>\r\n"
"        <MODULEPATH id=\"juce_osc\" path=\"%JUCE_PATH%\"/>\r\n"
"        <MODULEPATH id=\"hi_dsp_library\" path=\"%HISE_PATH%\"/>\r\n"
"        <MODULEPATH id=\"hi_frontend\" path=\"%HISE_PATH%\"/>\r\n"
"        <MODULEPATH id=\"hi_faust\" path=\"%HISE_PATH%\"/>\r\n"
"        <MODULEPATH id=\"hi_faust_types\" path=\"%HISE_PATH%\"/>\r\n"
"        <MODULEPATH id=\"hi_scripting\" path=\"%HISE_PATH%\"/>\r\n"
"        <MODULEPATH id=\"hi_core\" path=\"%HISE_PATH%\"/>\r\n"
"        <MODULEPATH id=\"hi_lac\" path=\"%HISE_PATH%\"/>\r\n"
"        <MODULEPATH id=\"hi_rlottie\" path=\"%HISE_PATH%\"/>\r\n"
"        <MODULEPATH id=\"hi_zstd\" path=\"%HISE_PATH%\"/>\r\n"
"        <MODULEPATH id=\"hi_streaming\" path=\"%HISE_PATH%\"/>\r\n"
"        <MODULEPATH id=\"hi_tools\" path=\"%HISE_PATH%\"/>\r\n"
"        <MODULEPATH id=\"hi_snex\" path=\"%HISE_PATH%\"/>\r\n"
"      </MODULEPATHS>\r\n"
"    </XCODE_MAC>\r\n"
"   <LINUX_MAKE targetFolder=\"Builds/LinuxMakefile\" linuxExtraPkgConfig=\"%LINUX_GUI_LIBS%\" extraLinkerFlags=\"-no-pie&#10;%IPP_COMPILER_FLAGS%\" extraCompilerFlags=\"-fpermissive\" extraDefs=\"%EXTRA_DEFINES_LINUX%\">\r\n"
"      <CONFIGURATIONS>\r\n"
"        <CONFIGURATION name=\"Debug\" isDebug=\"1\" optimisation=\"1\" targetName=\"%NAME%\" headerPath=\"%IPP_HEADER%;%FAUST_HEADER_PATH%\" libraryPath=\"%IPP_LIBRARY%\"/>\r\n"
"        <CONFIGURATION name=\"Release\" isDebug=\"0\" optimisation=\"3\" targetName=\"%NAME%\" headerPath=\"%IPP_HEADER%;%FAUST_HEADER_PATH%\" libraryPath=\"%IPP_LIBRARY%\" linkTimeOptimisation=\"%LINK_TIME_OPTIMISATION%\"/>\r\n"
"      </CONFIGURATIONS>\r\n"
"      <MODULEPATHS>\r\n"
"        <MODULEPATH id=\"juce_product_unlocking\" path=\"%HISE_PATH%\"/>\r\n"
"        <MODULEPATH id=\"juce_gui_extra\" path=\"%JUCE_PATH%\"/>\r\n"
"        <MODULEPATH id=\"juce_gui_basics\" path=\"%JUCE_PATH%\"/>\r\n"
"        <MODULEPATH id=\"juce_graphics\" path=\"%JUCE_PATH%\"/>\r\n"
"        <MODULEPATH id=\"juce_events\" path=\"%JUCE_PATH%\"/>\r\n"
"        <MODULEPATH id=\"juce_data_structures\" path=\"%JUCE_PATH%\"/>\r\n"
"        <MODULEPATH id=\"juce_dsp\" path=\"%JUCE_PATH%\"/>\r\n"
"        <MODULEPATH id=\"juce_cryptography\" path=\"%JUCE_PATH%\"/>\r\n"
"        <MODULEPATH id=\"juce_core\" path=\"%JUCE_PATH%\"/>\r\n"
"        <MODULEPATH id=\"juce_audio_utils\" path=\"%JUCE_PATH%\"/>\r\n"
"        <MODULEPATH id=\"juce_audio_processors\" path=\"%JUCE_PATH%\"/>\r\n"
"        <MODULEPATH id=\"juce_audio_formats\" path=\"%JUCE_PATH%\"/>\r\n"
"        <MODULEPATH id=\"juce_audio_devices\" path=\"%JUCE_PATH%\"/>\r\n"
"        <MODULEPATH id=\"juce_audio_basics\" path=\"%JUCE_PATH%\"/>\r\n"
"        <MODULEPATH id=\"juce_opengl\" path=\"%JUCE_PATH%\"/>\r\n"
"        <MODULEPATH id=\"juce_osc\" path=\"%JUCE_PATH%\"/>\r\n"
"        <MODULEPATH id=\"hi_scripting\" path=\"%HISE_PATH%\"/>\r\n"
"        <MODULEPATH id=\"hi_lac\" path=\"%HISE_PATH%\"/>\r\n"
"        <MODULEPATH id=\"hi_frontend\" path=\"%HISE_PATH%\"/>\r\n"
"        <MODULEPATH id=\"hi_dsp_library\" path=\"%HISE_PATH%\"/>\r\n"
"        <MODULEPATH id=\"hi_faust\" path=\"%HISE_PATH%\"/>\r\n"
"        <MODULEPATH id=\"hi_faust_types\" path=\"%HISE_PATH%\"/>\r\n"
"        <MODULEPATH id=\"hi_rlottie\" path=\"%HISE_PATH%\"/>\r\n"
"        <MODULEPATH id=\"hi_zstd\" path=\"%HISE_PATH%\"/>\r\n"
"        <MODULEPATH id=\"hi_streaming\" path=\"%HISE_PATH%\"/>\r\n"
"        <MODULEPATH id=\"hi_core\" path=\"%HISE_PATH%\"/>\r\n"
"        <MODULEPATH id=\"hi_tools\" path=\"%HISE_PATH%\"/>\r\n"
"        <MODULEPATH id=\"hi_snex\" path=\"%HISE_PATH%\"/>\r\n"
"      </MODULEPATHS>\r\n"
"    </LINUX_MAKE>\r\n"
"  </EXPORTFORMATS>\r\n"
"  <MODULES>\r\n"
"    <MODULE id=\"hi_core\" showAllCode=\"1\" useLocalCopy=\"0\"/>\r\n"
"    <MODULE id=\"hi_dsp_library\" showAllCode=\"1\" useLocalCopy=\"0\"/>\r\n"
"    <MODULE id=\"hi_faust\" showAllCode=\"1\" useLocalCopy=\"0\"/>\r\n"
"    <MODULE id=\"hi_faust_types\" showAllCode=\"1\" useLocalCopy=\"0\"/>\r\n"
"    <MODULE id=\"hi_frontend\" showAllCode=\"1\" useLocalCopy=\"0\"/>\r\n"
"    <MODULE id=\"hi_scripting\" showAllCode=\"1\" useLocalCopy=\"0\"/>\r\n"
"    <MODULE id=\"hi_lac\" showAllCode=\"1\" useLocalCopy=\"0\"/>\r\n"
"    <MODULE id=\"hi_zstd\" showAllCode=\"1\" useLocalCopy=\"0\"/>\r\n"
"    <MODULE id=\"hi_rlottie\" showAllCode=\"1\" useLocalCopy=\"0\"/>\r\n"
"    <MODULE id=\"hi_streaming\" showAllCode=\"1\" useLocalCopy=\"0\"/>\r\n"
"    <MODULE id=\"hi_tools\" showAllCode=\"1\" useLocalCopy=\"0\"/>\r\n"
"    <MODULE id=\"juce_audio_basics\" showAllCode=\"1\" useLocalCopy=\"0\"/>\r\n"
"    <MODULE id=\"juce_audio_devices\" showAllCode=\"1\" useLocalCopy=\"0\"/>\r\n"
"    <MODULE id=\"juce_audio_formats\" showAllCode=\"1\" useLocalCopy=\"0\"/>\r\n"
"    <MODULE id=\"juce_audio_processors\" showAllCode=\"1\" useLocalCopy=\"0\"/>\r\n"
"    <MODULE id=\"juce_audio_utils\" showAllCode=\"1\" useLocalCopy=\"0\"/>\r\n"
"    <MODULE id=\"juce_core\" showAllCode=\"1\" useLocalCopy=\"0\"/>\r\n"
"    <MODULE id=\"juce_cryptography\" showAllCode=\"1\" useLocalCopy=\"0\"/>\r\n"
"    <MODULE id=\"juce_data_structures\" showAllCode=\"1\" useLocalCopy=\"0\"/>\r\n"
"    <MODULE id=\"juce_dsp\" showAllCode=\"1\" useLocalCopy=\"0\"/>\r\n"
"    <MODULE id=\"juce_events\" showAllCode=\"1\" useLocalCopy=\"0\"/>\r\n"
"    <MODULE id=\"juce_graphics\" showAllCode=\"1\" useLocalCopy=\"0\"/>\r\n"
"    <MODULE id=\"juce_gui_basics\" showAllCode=\"1\" useLocalCopy=\"0\"/>\r\n"
"    <MODULE id=\"juce_gui_extra\" showAllCode=\"1\" useLocalCopy=\"0\"/>\r\n"
"    <MODULE id=\"juce_product_unlocking\" showAllCode=\"1\" useLocalCopy=\"0\"/>\r\n"
"    <MODULE id=\"juce_opengl\" showAllCode=\"1\" useLocalCopy=\"0\"/>\r\n"
"    <MODULE id=\"juce_osc\" showAllCode=\"1\" useLocalCopy=\"0\"/>\r\n"
"    <MODULE id=\"hi_snex\" showAllCode=\"1\" useLocalCopy=\"0\" useGlobalPath=\"0\"/>\r\n"
"  </MODULES>\r\n"
"  <JUCEOPTIONS JUCE_QUICKTIME=\"disabled\" USE_BACKEND=\"disabled\" USE_FRONTEND=\"enabled\" USE_RAW_FRONTEND=\"%USE_RAW_FRONTEND%\"\r\n"
"               HI_EXPORT_DSP_LIBRARY=\"disabled\" USE_COPY_PROTECTION=\"%USE_COPY_PROTECTION%\"\r\n"
"               IS_STANDALONE_APP=\"enabled\" USE_IPP=\"%USE_IPP%\" FRONTEND_IS_PLUGIN=\"%FRONTEND_IS_PLUGIN%\"\r\n"
"               USE_CUSTOM_FRONTEND_TOOLBAR=\"%USE_CUSTOM_FRONTEND_TOOLBAR%\" HI_SUPPORT_FULL_DYNAMICS_HLAC=\"%SUPPORT_FULL_DYNAMICS%\" READ_ONLY_FACTORY_PRESETS=\"%READ_ONLY_FACTORY_PRESETS%\" IS_STANDALONE_FRONTEND=\"%IS_STANDALONE_FRONTEND%\" USE_GLITCH_DETECTION=\"enabled\"\r\n"
"               ENABLE_PLOTTER=\"disabled\" ENABLE_SCRIPTING_SAFE_CHECKS=\"disabled\"\r\n"
"               ENABLE_ALL_PEAK_METERS=\"disabled\" ENABLE_CONSOLE_OUTPUT=\"disabled\"\r\n"
"               JUCE_ASIO=\"%USE_ASIO%\" JUCE_JACK=\"%USE_JACK%\" USE_SPLASH_SCREEN=\"%USE_SPLASH_SCREEN%\" HISE_OVERWRITE_OLD_USER_PRESETS=\"%OVERWRITE_OLD_USER_PRESETS%\" HI_ENABLE_LEGACY_CPU_SUPPORT=\"%LEGACY_CPU_SUPPORT%\" HISE_INCLUDE_RLOTTIE=\"enabled\" HISE_INCLUDE_FAUST=\"%HISE_INCLUDE_FAUST%\" HISE_USE_SYSTEM_APP_DATA_FOLDER=\"%USE_GLOBAL_APP_FOLDER%\"  HLAC_MEASURE_DECODING_PERFORMANCE=\"disabled\" HLAC_DEBUG_LOG=\"disabled\" HLAC_INCLUDE_TEST_SUITE=\"disabled\" STANDALONE_STREAMING=\"disabled\""
"				JUCE_DSP_USE_INTEL_MKL=\"disabled\" JUCE_WEB_BROWSER=\"disabled\" JUCE_USE_CURL=\"disabled\" JUCE_DSP_USE_SHARED_FFTW=\"disabled\" JUCE_DSP_USE_STATIC_FFTW=\"disabled\" HISE_USE_CUSTOM_EXPANSION_TYPE=\"%USE_CUSTOM_EXPANSION_TYPE%\" />\r\n"
"</JUCERPROJECT>\r\n";

const char* projectStandaloneTemplate_jucer = (const char*)projectStandaloneTemplate_jucer_lines;

} // namespace hise
