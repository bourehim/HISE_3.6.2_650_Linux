# Automatically generated makefile, created by the Projucer
# Don't edit this file! Your changes will be overwritten when you re-save the Projucer project!

# build with "V=1" for verbose builds
ifeq ($(V), 1)
V_AT =
else
V_AT = @
endif

# (this disables dependency generation if multiple architectures are set)
DEPFLAGS := $(if $(word 2, $(TARGET_ARCH)), , -MMD)

ifndef PKG_CONFIG
  PKG_CONFIG=pkg-config
endif

ifndef STRIP
  STRIP=strip
endif

ifndef AR
  AR=ar
endif

ifndef CONFIG
  CONFIG=Debug
endif

JUCE_ARCH_LABEL := $(shell uname -m)

ifeq ($(CONFIG),Debug)
  JUCE_BINDIR := build
  JUCE_LIBDIR := build
  JUCE_OBJDIR := build/intermediate/Debug
  JUCE_OUTDIR := build

  ifeq ($(TARGET_ARCH),)
    TARGET_ARCH := 
  endif

  JUCE_CPPFLAGS := $(DEPFLAGS) "-DLINUX=1" "-DNDEBUG=1" "-DJUCER_LINUX_MAKE_6D53C8B4=1" "-DJUCE_APP_VERSION=3.6.2" "-DJUCE_APP_VERSION_HEX=0x30602" $(shell $(PKG_CONFIG) --cflags alsa freetype2 webkit2gtk-4.0 gtk+-x11-3.0) -pthread -I../../JuceLibraryCode -I../../../../../HISE -I../../../../JUCE/modules $(CPPFLAGS)
  JUCE_CPPFLAGS_APP :=  "-DJucePlugin_Build_VST=0" "-DJucePlugin_Build_VST3=0" "-DJucePlugin_Build_AU=0" "-DJucePlugin_Build_AUv3=0" "-DJucePlugin_Build_AAX=0" "-DJucePlugin_Build_Standalone=0" "-DJucePlugin_Build_Unity=0" "-DJucePlugin_Build_LV2=0"
  JUCE_TARGET_APP := HISE\ Standalone

  JUCE_CFLAGS += $(JUCE_CPPFLAGS) $(TARGET_ARCH) -O3 $(CFLAGS)
  JUCE_CXXFLAGS += $(JUCE_CFLAGS) -std=c++17 $(CXXFLAGS)
  JUCE_LDFLAGS += $(TARGET_ARCH) -L$(JUCE_BINDIR) -L$(JUCE_LIBDIR) $(shell $(PKG_CONFIG) --libs alsa freetype2) -fvisibility=hidden -lrt -ldl -lpthread -lGL $(LDFLAGS)

  CLEANCMD = rm -rf $(JUCE_OUTDIR)/$(TARGET) $(JUCE_OBJDIR)
endif

ifeq ($(CONFIG),Release)
  JUCE_BINDIR := build
  JUCE_LIBDIR := build
  JUCE_OBJDIR := build/intermediate/Release
  JUCE_OUTDIR := build

  ifeq ($(TARGET_ARCH),)
    TARGET_ARCH := 
  endif

  JUCE_CPPFLAGS := $(DEPFLAGS) "-DLINUX=1" "-DNDEBUG=1" "-DJUCER_LINUX_MAKE_6D53C8B4=1" "-DJUCE_APP_VERSION=3.6.2" "-DJUCE_APP_VERSION_HEX=0x30602" $(shell $(PKG_CONFIG) --cflags alsa freetype2 webkit2gtk-4.0 gtk+-x11-3.0) -pthread -I../../JuceLibraryCode -I../../../../../HISE -I../../../../JUCE/modules $(CPPFLAGS)
  JUCE_CPPFLAGS_APP :=  "-DJucePlugin_Build_VST=0" "-DJucePlugin_Build_VST3=0" "-DJucePlugin_Build_AU=0" "-DJucePlugin_Build_AUv3=0" "-DJucePlugin_Build_AAX=0" "-DJucePlugin_Build_Standalone=0" "-DJucePlugin_Build_Unity=0" "-DJucePlugin_Build_LV2=0"
  JUCE_TARGET_APP := HISE\ Standalone

  JUCE_CFLAGS += $(JUCE_CPPFLAGS) $(TARGET_ARCH) -O3 $(CFLAGS)
  JUCE_CXXFLAGS += $(JUCE_CFLAGS) -std=c++17 $(CXXFLAGS)
  JUCE_LDFLAGS += $(TARGET_ARCH) -L$(JUCE_BINDIR) -L$(JUCE_LIBDIR) $(shell $(PKG_CONFIG) --libs alsa freetype2) -fvisibility=hidden -lrt -ldl -lpthread -lGL $(LDFLAGS)

  CLEANCMD = rm -rf $(JUCE_OUTDIR)/$(TARGET) $(JUCE_OBJDIR)
endif

OBJECTS_APP := \
  $(JUCE_OBJDIR)/ScriptNodeTestSuite_6b3f695a.o \
  $(JUCE_OBJDIR)/DspUnitTests_8fd29654.o \
  $(JUCE_OBJDIR)/HiseEventBufferUnitTests_fc3efacf.o \
  $(JUCE_OBJDIR)/MainComponent_a6ffb4a5.o \
  $(JUCE_OBJDIR)/Main_90ebc5c2.o \
  $(JUCE_OBJDIR)/BinaryData_ce4232d4.o \
  $(JUCE_OBJDIR)/include_hi_backend_377acbb6.o \
  $(JUCE_OBJDIR)/include_hi_core_ff8d8d87.o \
  $(JUCE_OBJDIR)/include_hi_dsp_library_01_6050d6b.o \
  $(JUCE_OBJDIR)/include_hi_dsp_library_02_61324ec.o \
  $(JUCE_OBJDIR)/include_hi_faust_jit_55188733.o \
  $(JUCE_OBJDIR)/include_hi_faust_lib_bde756a3.o \
  $(JUCE_OBJDIR)/include_hi_faust_lib_02_119f0228.o \
  $(JUCE_OBJDIR)/include_hi_lac_15d99830.o \
  $(JUCE_OBJDIR)/include_hi_lac_02_7a39397b.o \
  $(JUCE_OBJDIR)/include_hi_rlottie_b010f133.o \
  $(JUCE_OBJDIR)/include_hi_rlottie_1_40582c65.o \
  $(JUCE_OBJDIR)/include_hi_rlottie_2_406643e6.o \
  $(JUCE_OBJDIR)/include_hi_rlottie_3_40745b67.o \
  $(JUCE_OBJDIR)/include_hi_rlottie_4_408272e8.o \
  $(JUCE_OBJDIR)/include_hi_rlottie_5_40908a69.o \
  $(JUCE_OBJDIR)/include_hi_rlottie_6_409ea1ea.o \
  $(JUCE_OBJDIR)/include_hi_rlottie_7_40acb96b.o \
  $(JUCE_OBJDIR)/include_hi_rlottie_8_40bad0ec.o \
  $(JUCE_OBJDIR)/include_hi_rlottie_9_40c8e86d.o \
  $(JUCE_OBJDIR)/include_hi_rlottie_10_cab14735.o \
  $(JUCE_OBJDIR)/include_hi_rlottie_11_cabf5eb6.o \
  $(JUCE_OBJDIR)/include_hi_rlottie_12_cacd7637.o \
  $(JUCE_OBJDIR)/include_hi_rlottie_13_cadb8db8.o \
  $(JUCE_OBJDIR)/include_hi_rlottie_14_cae9a539.o \
  $(JUCE_OBJDIR)/include_hi_rlottie_15_caf7bcba.o \
  $(JUCE_OBJDIR)/include_hi_rlottie_16_cb05d43b.o \
  $(JUCE_OBJDIR)/include_hi_rlottie_17_cb13ebbc.o \
  $(JUCE_OBJDIR)/include_hi_rlottie_18_cb22033d.o \
  $(JUCE_OBJDIR)/include_hi_rlottie_19_cb301abe.o \
  $(JUCE_OBJDIR)/include_hi_rlottie_20_cc661fd4.o \
  $(JUCE_OBJDIR)/include_hi_rlottie_21_cc743755.o \
  $(JUCE_OBJDIR)/include_hi_rlottie_22_cc824ed6.o \
  $(JUCE_OBJDIR)/include_hi_rlottie_23_cc906657.o \
  $(JUCE_OBJDIR)/include_hi_rlottie_24_cc9e7dd8.o \
  $(JUCE_OBJDIR)/include_hi_rlottie_25_ccac9559.o \
  $(JUCE_OBJDIR)/include_hi_rlottie_26_ccbaacda.o \
  $(JUCE_OBJDIR)/include_hi_rlottie_27_ccc8c45b.o \
  $(JUCE_OBJDIR)/include_hi_rlottie_28_ccd6dbdc.o \
  $(JUCE_OBJDIR)/include_hi_rlottie_29_cce4f35d.o \
  $(JUCE_OBJDIR)/include_hi_rlottie_30_ce1af873.o \
  $(JUCE_OBJDIR)/include_hi_rlottie_31_ce290ff4.o \
  $(JUCE_OBJDIR)/include_hi_rlottie_32_ce372775.o \
  $(JUCE_OBJDIR)/include_hi_rlottie_33_ce453ef6.o \
  $(JUCE_OBJDIR)/include_hi_rlottie_34_ce535677.o \
  $(JUCE_OBJDIR)/include_hi_rlottie_35_ce616df8.o \
  $(JUCE_OBJDIR)/include_hi_scripting_01_6ad431f1.o \
  $(JUCE_OBJDIR)/include_hi_scripting_02_6ae24972.o \
  $(JUCE_OBJDIR)/include_hi_scripting_03_6af060f3.o \
  $(JUCE_OBJDIR)/include_hi_scripting_04_6afe7874.o \
  $(JUCE_OBJDIR)/include_hi_snex_3396deb6.o \
  $(JUCE_OBJDIR)/include_hi_snex_60_41e20b2d.o \
  $(JUCE_OBJDIR)/include_hi_snex_61_41e20eee.o \
  $(JUCE_OBJDIR)/include_hi_snex_62_51a830ef.o \
  $(JUCE_OBJDIR)/include_hi_streaming_6963df44.o \
  $(JUCE_OBJDIR)/include_hi_tools_3ba2cabd.o \
  $(JUCE_OBJDIR)/include_hi_zstd_1_81a60803.o \
  $(JUCE_OBJDIR)/include_hi_zstd_2_81b41f84.o \
  $(JUCE_OBJDIR)/include_hi_zstd_3_81c23705.o \
  $(JUCE_OBJDIR)/include_juce_audio_basics_8a4e984a.o \
  $(JUCE_OBJDIR)/include_juce_audio_devices_63111d02.o \
  $(JUCE_OBJDIR)/include_juce_audio_formats_15f82001.o \
  $(JUCE_OBJDIR)/include_juce_audio_processors_10c03666.o \
  $(JUCE_OBJDIR)/include_juce_audio_utils_9f9fb2d6.o \
  $(JUCE_OBJDIR)/include_juce_core_f26d17db.o \
  $(JUCE_OBJDIR)/include_juce_cryptography_8cb807a8.o \
  $(JUCE_OBJDIR)/include_juce_data_structures_7471b1e3.o \
  $(JUCE_OBJDIR)/include_juce_dsp_aeb2060f.o \
  $(JUCE_OBJDIR)/include_juce_events_fd7d695.o \
  $(JUCE_OBJDIR)/include_juce_graphics_f817e147.o \
  $(JUCE_OBJDIR)/include_juce_gui_basics_e3f79785.o \
  $(JUCE_OBJDIR)/include_juce_gui_extra_6dee1c1a.o \
  $(JUCE_OBJDIR)/include_juce_opengl_a8a032b.o \
  $(JUCE_OBJDIR)/include_juce_osc_f3df604d.o \
  $(JUCE_OBJDIR)/include_juce_product_unlocking_8278fcdc.o \

.PHONY: clean all strip

all : $(JUCE_OUTDIR)/$(JUCE_TARGET_APP)

$(JUCE_OUTDIR)/$(JUCE_TARGET_APP) : $(OBJECTS_APP) $(JUCE_OBJDIR)/execinfo.cmd $(RESOURCES)
	@command -v $(PKG_CONFIG) >/dev/null 2>&1 || { echo >&2 "pkg-config not installed. Please, install it."; exit 1; }
	@$(PKG_CONFIG) --print-errors alsa freetype2
	@echo Linking "HISE Standalone - App"
	-$(V_AT)mkdir -p $(JUCE_BINDIR)
	-$(V_AT)mkdir -p $(JUCE_LIBDIR)
	-$(V_AT)mkdir -p $(JUCE_OUTDIR)
	$(V_AT)$(CXX) -o $(JUCE_OUTDIR)/$(JUCE_TARGET_APP) $(OBJECTS_APP) $(JUCE_LDFLAGS) $(shell cat $(JUCE_OBJDIR)/execinfo.cmd) $(JUCE_LDFLAGS_APP) $(RESOURCES) $(TARGET_ARCH)

$(JUCE_OBJDIR)/ScriptNodeTestSuite_6b3f695a.o: ../../../../hi_scripting/scripting/scriptnode/ScriptNodeTestSuite.cpp
	-$(V_AT)mkdir -p $(@D)
	@echo "Compiling ScriptNodeTestSuite.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/DspUnitTests_8fd29654.o: ../../../../hi_scripting/scripting/api/DspUnitTests.cpp
	-$(V_AT)mkdir -p $(@D)
	@echo "Compiling DspUnitTests.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/HiseEventBufferUnitTests_fc3efacf.o: ../../../../hi_core/hi_core/HiseEventBufferUnitTests.cpp
	-$(V_AT)mkdir -p $(@D)
	@echo "Compiling HiseEventBufferUnitTests.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/MainComponent_a6ffb4a5.o: ../../Source/MainComponent.cpp
	-$(V_AT)mkdir -p $(@D)
	@echo "Compiling MainComponent.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/Main_90ebc5c2.o: ../../Source/Main.cpp
	-$(V_AT)mkdir -p $(@D)
	@echo "Compiling Main.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/BinaryData_ce4232d4.o: ../../JuceLibraryCode/BinaryData.cpp
	-$(V_AT)mkdir -p $(@D)
	@echo "Compiling BinaryData.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/include_hi_backend_377acbb6.o: ../../JuceLibraryCode/include_hi_backend.cpp
	-$(V_AT)mkdir -p $(@D)
	@echo "Compiling include_hi_backend.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/include_hi_core_ff8d8d87.o: ../../JuceLibraryCode/include_hi_core.cpp
	-$(V_AT)mkdir -p $(@D)
	@echo "Compiling include_hi_core.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/include_hi_dsp_library_01_6050d6b.o: ../../JuceLibraryCode/include_hi_dsp_library_01.cpp
	-$(V_AT)mkdir -p $(@D)
	@echo "Compiling include_hi_dsp_library_01.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/include_hi_dsp_library_02_61324ec.o: ../../JuceLibraryCode/include_hi_dsp_library_02.cpp
	-$(V_AT)mkdir -p $(@D)
	@echo "Compiling include_hi_dsp_library_02.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/include_hi_faust_jit_55188733.o: ../../JuceLibraryCode/include_hi_faust_jit.cpp
	-$(V_AT)mkdir -p $(@D)
	@echo "Compiling include_hi_faust_jit.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/include_hi_faust_lib_bde756a3.o: ../../JuceLibraryCode/include_hi_faust_lib.cpp
	-$(V_AT)mkdir -p $(@D)
	@echo "Compiling include_hi_faust_lib.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/include_hi_faust_lib_02_119f0228.o: ../../JuceLibraryCode/include_hi_faust_lib_02.cpp
	-$(V_AT)mkdir -p $(@D)
	@echo "Compiling include_hi_faust_lib_02.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/include_hi_lac_15d99830.o: ../../JuceLibraryCode/include_hi_lac.cpp
	-$(V_AT)mkdir -p $(@D)
	@echo "Compiling include_hi_lac.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/include_hi_lac_02_7a39397b.o: ../../JuceLibraryCode/include_hi_lac_02.cpp
	-$(V_AT)mkdir -p $(@D)
	@echo "Compiling include_hi_lac_02.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/include_hi_rlottie_b010f133.o: ../../JuceLibraryCode/include_hi_rlottie.cpp
	-$(V_AT)mkdir -p $(@D)
	@echo "Compiling include_hi_rlottie.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/include_hi_rlottie_1_40582c65.o: ../../JuceLibraryCode/include_hi_rlottie_1.cpp
	-$(V_AT)mkdir -p $(@D)
	@echo "Compiling include_hi_rlottie_1.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/include_hi_rlottie_2_406643e6.o: ../../JuceLibraryCode/include_hi_rlottie_2.cpp
	-$(V_AT)mkdir -p $(@D)
	@echo "Compiling include_hi_rlottie_2.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/include_hi_rlottie_3_40745b67.o: ../../JuceLibraryCode/include_hi_rlottie_3.cpp
	-$(V_AT)mkdir -p $(@D)
	@echo "Compiling include_hi_rlottie_3.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/include_hi_rlottie_4_408272e8.o: ../../JuceLibraryCode/include_hi_rlottie_4.cpp
	-$(V_AT)mkdir -p $(@D)
	@echo "Compiling include_hi_rlottie_4.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/include_hi_rlottie_5_40908a69.o: ../../JuceLibraryCode/include_hi_rlottie_5.cpp
	-$(V_AT)mkdir -p $(@D)
	@echo "Compiling include_hi_rlottie_5.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/include_hi_rlottie_6_409ea1ea.o: ../../JuceLibraryCode/include_hi_rlottie_6.cpp
	-$(V_AT)mkdir -p $(@D)
	@echo "Compiling include_hi_rlottie_6.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/include_hi_rlottie_7_40acb96b.o: ../../JuceLibraryCode/include_hi_rlottie_7.cpp
	-$(V_AT)mkdir -p $(@D)
	@echo "Compiling include_hi_rlottie_7.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/include_hi_rlottie_8_40bad0ec.o: ../../JuceLibraryCode/include_hi_rlottie_8.cpp
	-$(V_AT)mkdir -p $(@D)
	@echo "Compiling include_hi_rlottie_8.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/include_hi_rlottie_9_40c8e86d.o: ../../JuceLibraryCode/include_hi_rlottie_9.cpp
	-$(V_AT)mkdir -p $(@D)
	@echo "Compiling include_hi_rlottie_9.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/include_hi_rlottie_10_cab14735.o: ../../JuceLibraryCode/include_hi_rlottie_10.cpp
	-$(V_AT)mkdir -p $(@D)
	@echo "Compiling include_hi_rlottie_10.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/include_hi_rlottie_11_cabf5eb6.o: ../../JuceLibraryCode/include_hi_rlottie_11.cpp
	-$(V_AT)mkdir -p $(@D)
	@echo "Compiling include_hi_rlottie_11.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/include_hi_rlottie_12_cacd7637.o: ../../JuceLibraryCode/include_hi_rlottie_12.cpp
	-$(V_AT)mkdir -p $(@D)
	@echo "Compiling include_hi_rlottie_12.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/include_hi_rlottie_13_cadb8db8.o: ../../JuceLibraryCode/include_hi_rlottie_13.cpp
	-$(V_AT)mkdir -p $(@D)
	@echo "Compiling include_hi_rlottie_13.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/include_hi_rlottie_14_cae9a539.o: ../../JuceLibraryCode/include_hi_rlottie_14.cpp
	-$(V_AT)mkdir -p $(@D)
	@echo "Compiling include_hi_rlottie_14.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/include_hi_rlottie_15_caf7bcba.o: ../../JuceLibraryCode/include_hi_rlottie_15.cpp
	-$(V_AT)mkdir -p $(@D)
	@echo "Compiling include_hi_rlottie_15.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/include_hi_rlottie_16_cb05d43b.o: ../../JuceLibraryCode/include_hi_rlottie_16.cpp
	-$(V_AT)mkdir -p $(@D)
	@echo "Compiling include_hi_rlottie_16.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/include_hi_rlottie_17_cb13ebbc.o: ../../JuceLibraryCode/include_hi_rlottie_17.cpp
	-$(V_AT)mkdir -p $(@D)
	@echo "Compiling include_hi_rlottie_17.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/include_hi_rlottie_18_cb22033d.o: ../../JuceLibraryCode/include_hi_rlottie_18.cpp
	-$(V_AT)mkdir -p $(@D)
	@echo "Compiling include_hi_rlottie_18.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/include_hi_rlottie_19_cb301abe.o: ../../JuceLibraryCode/include_hi_rlottie_19.cpp
	-$(V_AT)mkdir -p $(@D)
	@echo "Compiling include_hi_rlottie_19.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/include_hi_rlottie_20_cc661fd4.o: ../../JuceLibraryCode/include_hi_rlottie_20.cpp
	-$(V_AT)mkdir -p $(@D)
	@echo "Compiling include_hi_rlottie_20.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/include_hi_rlottie_21_cc743755.o: ../../JuceLibraryCode/include_hi_rlottie_21.cpp
	-$(V_AT)mkdir -p $(@D)
	@echo "Compiling include_hi_rlottie_21.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/include_hi_rlottie_22_cc824ed6.o: ../../JuceLibraryCode/include_hi_rlottie_22.cpp
	-$(V_AT)mkdir -p $(@D)
	@echo "Compiling include_hi_rlottie_22.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/include_hi_rlottie_23_cc906657.o: ../../JuceLibraryCode/include_hi_rlottie_23.cpp
	-$(V_AT)mkdir -p $(@D)
	@echo "Compiling include_hi_rlottie_23.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/include_hi_rlottie_24_cc9e7dd8.o: ../../JuceLibraryCode/include_hi_rlottie_24.cpp
	-$(V_AT)mkdir -p $(@D)
	@echo "Compiling include_hi_rlottie_24.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/include_hi_rlottie_25_ccac9559.o: ../../JuceLibraryCode/include_hi_rlottie_25.cpp
	-$(V_AT)mkdir -p $(@D)
	@echo "Compiling include_hi_rlottie_25.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/include_hi_rlottie_26_ccbaacda.o: ../../JuceLibraryCode/include_hi_rlottie_26.cpp
	-$(V_AT)mkdir -p $(@D)
	@echo "Compiling include_hi_rlottie_26.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/include_hi_rlottie_27_ccc8c45b.o: ../../JuceLibraryCode/include_hi_rlottie_27.cpp
	-$(V_AT)mkdir -p $(@D)
	@echo "Compiling include_hi_rlottie_27.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/include_hi_rlottie_28_ccd6dbdc.o: ../../JuceLibraryCode/include_hi_rlottie_28.cpp
	-$(V_AT)mkdir -p $(@D)
	@echo "Compiling include_hi_rlottie_28.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/include_hi_rlottie_29_cce4f35d.o: ../../JuceLibraryCode/include_hi_rlottie_29.cpp
	-$(V_AT)mkdir -p $(@D)
	@echo "Compiling include_hi_rlottie_29.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/include_hi_rlottie_30_ce1af873.o: ../../JuceLibraryCode/include_hi_rlottie_30.cpp
	-$(V_AT)mkdir -p $(@D)
	@echo "Compiling include_hi_rlottie_30.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/include_hi_rlottie_31_ce290ff4.o: ../../JuceLibraryCode/include_hi_rlottie_31.cpp
	-$(V_AT)mkdir -p $(@D)
	@echo "Compiling include_hi_rlottie_31.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/include_hi_rlottie_32_ce372775.o: ../../JuceLibraryCode/include_hi_rlottie_32.cpp
	-$(V_AT)mkdir -p $(@D)
	@echo "Compiling include_hi_rlottie_32.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/include_hi_rlottie_33_ce453ef6.o: ../../JuceLibraryCode/include_hi_rlottie_33.cpp
	-$(V_AT)mkdir -p $(@D)
	@echo "Compiling include_hi_rlottie_33.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/include_hi_rlottie_34_ce535677.o: ../../JuceLibraryCode/include_hi_rlottie_34.cpp
	-$(V_AT)mkdir -p $(@D)
	@echo "Compiling include_hi_rlottie_34.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/include_hi_rlottie_35_ce616df8.o: ../../JuceLibraryCode/include_hi_rlottie_35.cpp
	-$(V_AT)mkdir -p $(@D)
	@echo "Compiling include_hi_rlottie_35.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/include_hi_scripting_01_6ad431f1.o: ../../JuceLibraryCode/include_hi_scripting_01.cpp
	-$(V_AT)mkdir -p $(@D)
	@echo "Compiling include_hi_scripting_01.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/include_hi_scripting_02_6ae24972.o: ../../JuceLibraryCode/include_hi_scripting_02.cpp
	-$(V_AT)mkdir -p $(@D)
	@echo "Compiling include_hi_scripting_02.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/include_hi_scripting_03_6af060f3.o: ../../JuceLibraryCode/include_hi_scripting_03.cpp
	-$(V_AT)mkdir -p $(@D)
	@echo "Compiling include_hi_scripting_03.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/include_hi_scripting_04_6afe7874.o: ../../JuceLibraryCode/include_hi_scripting_04.cpp
	-$(V_AT)mkdir -p $(@D)
	@echo "Compiling include_hi_scripting_04.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/include_hi_snex_3396deb6.o: ../../JuceLibraryCode/include_hi_snex.cpp
	-$(V_AT)mkdir -p $(@D)
	@echo "Compiling include_hi_snex.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/include_hi_snex_60_41e20b2d.o: ../../JuceLibraryCode/include_hi_snex_60.c
	-$(V_AT)mkdir -p $(@D)
	@echo "Compiling include_hi_snex_60.c"
	$(V_AT)$(CC) $(JUCE_CFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/include_hi_snex_61_41e20eee.o: ../../JuceLibraryCode/include_hi_snex_61.c
	-$(V_AT)mkdir -p $(@D)
	@echo "Compiling include_hi_snex_61.c"
	$(V_AT)$(CC) $(JUCE_CFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/include_hi_snex_62_51a830ef.o: ../../JuceLibraryCode/include_hi_snex_62.cpp
	-$(V_AT)mkdir -p $(@D)
	@echo "Compiling include_hi_snex_62.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/include_hi_streaming_6963df44.o: ../../JuceLibraryCode/include_hi_streaming.cpp
	-$(V_AT)mkdir -p $(@D)
	@echo "Compiling include_hi_streaming.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/include_hi_tools_3ba2cabd.o: ../../JuceLibraryCode/include_hi_tools.cpp
	-$(V_AT)mkdir -p $(@D)
	@echo "Compiling include_hi_tools.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/include_hi_zstd_1_81a60803.o: ../../JuceLibraryCode/include_hi_zstd_1.cpp
	-$(V_AT)mkdir -p $(@D)
	@echo "Compiling include_hi_zstd_1.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/include_hi_zstd_2_81b41f84.o: ../../JuceLibraryCode/include_hi_zstd_2.cpp
	-$(V_AT)mkdir -p $(@D)
	@echo "Compiling include_hi_zstd_2.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/include_hi_zstd_3_81c23705.o: ../../JuceLibraryCode/include_hi_zstd_3.cpp
	-$(V_AT)mkdir -p $(@D)
	@echo "Compiling include_hi_zstd_3.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/include_juce_audio_basics_8a4e984a.o: ../../JuceLibraryCode/include_juce_audio_basics.cpp
	-$(V_AT)mkdir -p $(@D)
	@echo "Compiling include_juce_audio_basics.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/include_juce_audio_devices_63111d02.o: ../../JuceLibraryCode/include_juce_audio_devices.cpp
	-$(V_AT)mkdir -p $(@D)
	@echo "Compiling include_juce_audio_devices.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/include_juce_audio_formats_15f82001.o: ../../JuceLibraryCode/include_juce_audio_formats.cpp
	-$(V_AT)mkdir -p $(@D)
	@echo "Compiling include_juce_audio_formats.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/include_juce_audio_processors_10c03666.o: ../../JuceLibraryCode/include_juce_audio_processors.cpp
	-$(V_AT)mkdir -p $(@D)
	@echo "Compiling include_juce_audio_processors.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/include_juce_audio_utils_9f9fb2d6.o: ../../JuceLibraryCode/include_juce_audio_utils.cpp
	-$(V_AT)mkdir -p $(@D)
	@echo "Compiling include_juce_audio_utils.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/include_juce_core_f26d17db.o: ../../JuceLibraryCode/include_juce_core.cpp
	-$(V_AT)mkdir -p $(@D)
	@echo "Compiling include_juce_core.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/include_juce_cryptography_8cb807a8.o: ../../JuceLibraryCode/include_juce_cryptography.cpp
	-$(V_AT)mkdir -p $(@D)
	@echo "Compiling include_juce_cryptography.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/include_juce_data_structures_7471b1e3.o: ../../JuceLibraryCode/include_juce_data_structures.cpp
	-$(V_AT)mkdir -p $(@D)
	@echo "Compiling include_juce_data_structures.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/include_juce_dsp_aeb2060f.o: ../../JuceLibraryCode/include_juce_dsp.cpp
	-$(V_AT)mkdir -p $(@D)
	@echo "Compiling include_juce_dsp.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/include_juce_events_fd7d695.o: ../../JuceLibraryCode/include_juce_events.cpp
	-$(V_AT)mkdir -p $(@D)
	@echo "Compiling include_juce_events.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/include_juce_graphics_f817e147.o: ../../JuceLibraryCode/include_juce_graphics.cpp
	-$(V_AT)mkdir -p $(@D)
	@echo "Compiling include_juce_graphics.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/include_juce_gui_basics_e3f79785.o: ../../JuceLibraryCode/include_juce_gui_basics.cpp
	-$(V_AT)mkdir -p $(@D)
	@echo "Compiling include_juce_gui_basics.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/include_juce_gui_extra_6dee1c1a.o: ../../JuceLibraryCode/include_juce_gui_extra.cpp
	-$(V_AT)mkdir -p $(@D)
	@echo "Compiling include_juce_gui_extra.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/include_juce_opengl_a8a032b.o: ../../JuceLibraryCode/include_juce_opengl.cpp
	-$(V_AT)mkdir -p $(@D)
	@echo "Compiling include_juce_opengl.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/include_juce_osc_f3df604d.o: ../../JuceLibraryCode/include_juce_osc.cpp
	-$(V_AT)mkdir -p $(@D)
	@echo "Compiling include_juce_osc.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/include_juce_product_unlocking_8278fcdc.o: ../../JuceLibraryCode/include_juce_product_unlocking.cpp
	-$(V_AT)mkdir -p $(@D)
	@echo "Compiling include_juce_product_unlocking.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/execinfo.cmd:
	-$(V_AT)mkdir -p $(@D)
	-@if [ -z "$(V_AT)" ]; then echo "Checking if we need to link libexecinfo"; fi
	$(V_AT)printf "int main() { return 0; }" | $(CXX) -x c++ -o $(@D)/execinfo.x -lexecinfo - >/dev/null 2>&1 && printf -- "-lexecinfo" > "$@" || touch "$@"

$(JUCE_OBJDIR)/cxxfs.cmd:
	-$(V_AT)mkdir -p $(@D)
	-@if [ -z "$(V_AT)" ]; then echo "Checking if we need to link stdc++fs"; fi
	$(V_AT)printf "int main() { return 0; }" | $(CXX) -x c++ -o $(@D)/cxxfs.x -lstdc++fs - >/dev/null 2>&1 && printf -- "-lstdc++fs" > "$@" || touch "$@"

clean:
	@echo Cleaning HISE Standalone
	$(V_AT)$(CLEANCMD)

strip:
	@echo Stripping HISE Standalone
	-$(V_AT)$(STRIP) --strip-unneeded $(JUCE_OUTDIR)/$(TARGET)

-include $(OBJECTS_APP:%.o=%.d)
