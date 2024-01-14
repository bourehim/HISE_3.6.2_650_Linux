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

namespace hise { using namespace juce;




MARKDOWN_CHAPTER(WavetableHelp)

START_MARKDOWN(Mode)
ML("## Analysis mode")
ML("- **Resample**: this will not perform any resynthesis and just resamples a perfectly aligned wavetable audio file into a .hwt file");
ML("- **Loris**: this will use the Loris library and resynthesise the wavetables from the audio.");
END_MARKDOWN()

START_MARKDOWN(ReverseWavetables)
ML("## Reverse Wavetable order")
ML("If this is enabled, it will store the wavetables in reversed order.  ")
ML("This is useful if you have decaying samples and want to use the table index to modulate the decay process")
END_MARKDOWN()

START_MARKDOWN(Offset)
ML("## Transient detection")
ML("This setting defines how much the resynthesis should try to preserve transient and / or fast changing harmonics. Setting a low value here might result in smeared transients, but increases the frequency resolution.");
END_MARKDOWN();

START_MARKDOWN(PreservePhase)
ML("## Preserve Phase")
ML("This will preserve the phase when resynthesising the wavetables.  ")
ML("If the sound comes from a natural source, enabling this might improve the stereo image. ");
ML("For pure synthetic sounds this might create phasing issues");
END_MARKDOWN()

START_MARKDOWN(Slices)
ML("## Slices")
ML("This will determine how many wavetables will be extraced from the sample. ")
ML("> This setting only affects Zero Phase or Static Phase analysis, if you're using Dynamic Phase or Resample, the number of slices will be determined by the source file");
END_MARKDOWN()

START_MARKDOWN(About)
ML("### How to use this tool")
ML("1. Create a samplemap with the samples that you want to convert to a wavetable.You can use multiple samples for different notes, however the samplemap must not have multiple velocity layers.")
ML("2. Choose a Analysis mode that suits your source samples.")
ML("3. Play around with the different analysis and exporter options.You can click on the samplemap below to select different samples for previewing.")
ML("4. Click one of the Export buttons to create a.HWT file that can be loaded into the Wavetable Synthesiser or a samplemap with the processed sound(which is useful for noise separation or performing additional steps before the wavetable conversion).")
ML("> As soon as you've analysed your first sample, clicking on this button again will show the wavetable properties of the currently selected sample.")
END_MARKDOWN()

START_MARKDOWN(MipmapSize)
ML("## Mip Map size")
ML("Defines the interval that will be used to resynthesise the same sample.  ");
ML("In order to reduce aliasing, you can resynthesise the same sample at different pitches.  ");
ML("By default it is an octave but you can specify a lower amount for harmonicly rich signals");
END_MARKDOWN()

END_MARKDOWN_CHAPTER()


static bool canConnectToWebsite(const URL& url)
{
	auto in = url.createInputStream(false, nullptr, nullptr, String(), 2000, nullptr);
	return in != nullptr;
}

static bool areMajorWebsitesAvailable()
{
	const char* urlsToTry[] = { "http://google.com/generate_204", "https://amazon.com", nullptr };

	for (const char** url = urlsToTry; *url != nullptr; ++url)
	{
		URL u(*url);

		std::unique_ptr<InputStream> in(u.createInputStream(false, nullptr, nullptr, String(), HISE_SCRIPT_SERVER_TIMEOUT, nullptr));
		
		if (in != nullptr)
			return true;
	}

	return false;
}



class UpdateChecker : public DialogWindowWithBackgroundThread
{
public:

	class ScopedTempFile
	{
	public:

		ScopedTempFile(const File &f_) :
			f(f_)
		{
			f.deleteFile();

			jassert(!f.existsAsFile());

			f.create();
		}

		~ScopedTempFile()
		{
			jassert(f.existsAsFile());

			jassert(f.deleteFile());
		}


		File f;
	};

	UpdateChecker() :
		DialogWindowWithBackgroundThread("Checking for newer version."),
		updatesAvailable(false)
	{
		updatesAvailable = checkUpdate();

		if (updatesAvailable)
		{
			filePicker = new FilenameComponent("Download Location", File::getSpecialLocation(File::SpecialLocationType::userDesktopDirectory), false, true, true, "", "", "Choose Download Location");
			filePicker->setSize(500, 24);

			addCustomComponent(filePicker);

			addBasicComponents();

			showStatusMessage("New build available: " + newVersion + ". Press OK to download file to the selected location");
		}
		else
		{
			addBasicComponents(false);
			showStatusMessage("Your HISE build is up to date.");
		}
	}

	static bool downloadProgress(void* context, int bytesSent, int totalBytes)
	{
		const double downloadedMB = (double)bytesSent / 1024.0 / 1024.0;
		const double totalMB = (double)totalBytes / 1024.0 / 1024.0;
		const double percent = downloadedMB / totalMB;

		static_cast<UpdateChecker*>(context)->showStatusMessage("Downloaded: " + String(downloadedMB, 2) + " MB / " + String(totalMB, 2) + " MB");

		static_cast<UpdateChecker*>(context)->setProgress(percent);

		return !static_cast<UpdateChecker*>(context)->threadShouldExit();
	}

	void run()
	{
		auto assets = obj["assets"];

#if JUCE_WINDOWS
		auto extension = ".exe";
#elif JUCE_MAC
		auto extension = ".pkg";
#else
		auto extension = "david_has_to_build_it_himself";
#endif

		URL url;

		if(assets.isArray())
		{
			for(auto& a: *assets.getArray())
			{
				if(a["name"].toString().endsWith(extension))
				{
					url = URL(a["browser_download_url"].toString());
					break;
				}
			}
		}

		auto downloadFileName = url.getFileName();

		auto stream = url.createInputStream(false, &downloadProgress, this);

		target = File(filePicker->getCurrentFile().getChildFile(downloadFileName));

		if (!target.existsAsFile())
		{
			MemoryBlock mb;

			mb.setSize(8192);

			tempFile = new ScopedTempFile(File(target.getFullPathName() + "temp"));

			ScopedPointer<FileOutputStream> fos = new FileOutputStream(tempFile->f);

			const int64 numBytesTotal = stream->getNumBytesRemaining();

			int64 numBytesRead = 0;

			downloadOK = false;

			while (stream->getNumBytesRemaining() > 0)
			{
				const int64 chunkSize = (int64)jmin<int>((int)stream->getNumBytesRemaining(), 8192);

				downloadProgress(this, (int)numBytesRead, (int)numBytesTotal);

				if (threadShouldExit())
				{
					fos->flush();
					fos = nullptr;

					tempFile = nullptr;
					return;
				}

				stream->read(mb.getData(), (int)chunkSize);

				numBytesRead += chunkSize;

				fos->write(mb.getData(), (size_t)chunkSize);
			}

			downloadOK = true;
			fos->flush();

			tempFile->f.copyFileTo(target);
		}
	};

	void threadFinished()
	{
		if (downloadOK)
		{
			PresetHandler::showMessageWindow("Download finished", "Quit the app and run the installer to update to the latest version", PresetHandler::IconType::Info);

			target.revealToUser();
		}
	}

private:

	var obj;

	bool checkUpdate()
	{
		URL url("https://api.github.com");
		url = url.withNewSubPath("repos/christophhart/HISE/releases/latest");

		auto response = url.readEntireTextStream();

		obj = JSON::parse(response);

		if(obj.isObject())
		{
			newVersion = obj["tag_name"].toString();

			auto thisVersion = "3.4.9";// ProjectInfo::versionString;

			SemanticVersionChecker svs(thisVersion, newVersion);

			return svs.isUpdate();
		}

		return false;
		
	}

	String newVersion;

	bool updatesAvailable;

	File target;
	ScopedPointer<ScopedTempFile> tempFile;

	bool downloadOK;

	ScopedPointer<FilenameComponent> filePicker;
	ScopedPointer<TextEditor> changelogDisplay;
};




void XmlBackupFunctions::restoreAllScripts(ValueTree& v, ModulatorSynthChain *masterChain, const String &newId)
{
	static const Identifier pr("Processor");
	static const Identifier scr("Script");
	static const Identifier id("ID");
	static const Identifier typ("Type");

	if (v.getType() == Identifier(pr) && v[typ].toString().contains("Script"))
	{
		auto fileName = getSanitiziedName(v[id]);
		const String t = v[scr];
		
		if (t.startsWith("{EXTERNAL_SCRIPT}"))
		{
			return;
		}

		File scriptDirectory = getScriptDirectoryFor(masterChain, newId);

		auto sf = scriptDirectory.getChildFile(fileName).withFileExtension("js");

		if (!sf.existsAsFile())
		{
			auto isExternal = v.getProperty(scr).toString().startsWith("{EXTERNAL_SCRIPT}");

			if (!isExternal)
			{
				PresetHandler::showMessageWindow("Script not found", "Error loading script " + fileName, PresetHandler::IconType::Error);
			}
		}

        for(auto f: RangedDirectoryIterator(scriptDirectory, false, "*.js", File::findFiles))
		{
			File script = f.getFile();

			if (script.getFileNameWithoutExtension() == fileName)
			{
				v.setProperty(scr, script.loadFileAsString(), nullptr);
				break;
			}
		}
	}

	for (auto c: v)
		restoreAllScripts(c, masterChain, newId);
}

class DummyUnlocker : public OnlineUnlockStatus
{
public:

	DummyUnlocker(MainController *mc_) :
		mc(mc_)
	{

	}

	String getProductID() override
	{
		return dynamic_cast<GlobalSettingManager*>(mc)->getSettingsObject().getSetting(HiseSettings::Project::Name).toString();
	}

	bool doesProductIDMatch(const String & 	returnedIDFromServer)
	{
		return returnedIDFromServer == getProductID();
	}




private:

	MainController* mc;


};


class ProjectArchiver : public ThreadWithQuasiModalProgressWindow
{
public:

	ProjectArchiver(File &archiveFile_, File &projectDirectory_, ThreadWithQuasiModalProgressWindow::Holder *holder) :
		ThreadWithQuasiModalProgressWindow("Archiving Project", true, true, holder),
		archiveFile(archiveFile_),
		projectDirectory(projectDirectory_)
	{
		getAlertWindow()->setLookAndFeel(&alaf);
	}

	void run()
	{
		ZipFile::Builder builder;

		StringArray ignoredDirectories;

		ignoredDirectories.add("Binaries");
		ignoredDirectories.add("git");

		for(auto f: RangedDirectoryIterator(projectDirectory, true, "*", File::findFilesAndDirectories))
		{
			File currentFile = f.getFile();

			if (currentFile.isDirectory() ||
				currentFile.getFullPathName().contains("git") ||
				currentFile.getParentDirectory().getFullPathName().contains("Binaries"))
			{
				continue;
			}

			builder.addFile(currentFile, 9, currentFile.getRelativePathFrom(projectDirectory));
		}

		setStatusMessage("Creating ZIP archive of project folder");

		archiveFile.deleteFile();

		archiveFile.create();

		FileOutputStream fos(archiveFile);

		builder.writeToStream(fos, getProgressValue());


	}

	void threadComplete(bool userPressedCancel) override
	{
		if (!userPressedCancel && PresetHandler::showYesNoWindow("Successfully exported", "Press OK to show the archive", PresetHandler::IconType::Info))
		{
			archiveFile.revealToUser();
		}

		ThreadWithQuasiModalProgressWindow::threadComplete(userPressedCancel);
	}

private:

	AlertWindowLookAndFeel alaf;

	File archiveFile;

	File projectDirectory;
};



/** TODO:
 
    - add preview using Wavetable sound & actual voice rendering ...OK
	- make samplemap selectable ...OK
	- add waterfall display to converter dialog with scanner
    - add proper multithreading for rebuilding
    - show statistics
    - loris: add progress & status message manager
	- add loris DLL check
    - fix start & end
	- add zero wavetable at the end for fade to silence (with option)
    
 */
class WavetableConverterDialog : public DialogWindowWithBackgroundThread,
	public ComboBoxListener,
	public PathFactory

{
	struct Separator : public Component
	{
		Separator(const String& text_) :
			text(text_)
		{
			setSize(GLOBAL_BOLD_FONT().getStringWidthFloat(text) + 10, 32);
		}
		void paint(Graphics& g) override
		{
			g.setColour(Colours::white.withAlpha(0.4f));
			g.setFont(GLOBAL_BOLD_FONT());
			g.drawText(text, getLocalBounds().toFloat(), Justification::centred);
		}

		String text;
	};


	enum ButtonIds
	{
		Rescan,
		Discard,
		Preview,
		Original,
		numButtonIds
	};

	struct PreviewButton : public Button
	{
		PreviewButton(const String& name) :
			Button(name)
		{
			setClickingTogglesState(true);
			setSize(128, 32);
		}

		void setProgress(double d)
		{
			progress = d;
			
			SafeAsyncCall::repaint(this);
		}

		double progress = 1.0;

		void paintButton(Graphics& g, bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown) override
		{
			auto b = getLocalBounds().toFloat().reduced(5.0f, 8.0f);

			Path p;
			p.addTriangle({ 0.0f, 0.0f }, { 1.0f, 0.5f }, { 0.0f, 1.0f });

			

			auto tb = b.removeFromLeft(b.getHeight()).reduced(4.0f);

			p.scaleToFit(tb.getX(), tb.getY(), tb.getWidth(), tb.getHeight(), false);

			float alpha = 0.4f;

			if (shouldDrawButtonAsHighlighted)
				alpha += 0.1f;

			if (shouldDrawButtonAsDown)
				alpha += 0.4f;

			if (getToggleState())
			{
				g.setColour(Colours::white.withAlpha(0.05f));
				
				auto b = getLocalBounds().toFloat().reduced(0.0, 4.0f);
				
				g.fillRect(b);
				b = b.removeFromLeft(getWidth() * progress);
				g.fillRect(b);
			}

			g.setColour(Colours::white.withAlpha(alpha));
			g.fillPath(p);
			g.setFont(GLOBAL_BOLD_FONT());
			g.drawText(getName(), b, Justification::left);
		}
	};

public:
	WavetableConverterDialog(ModulatorSynthChain* chain_) :
		DialogWindowWithBackgroundThread("Convert Samplemaps to Wavetable"),
		chain(chain_),
		converter(new SampleMapToWavetableConverter(chain_)),
		r(Result::ok()),
		currentTasks(32),
		bl_(*this)
	{
		converter->setLogFunction([this](const String& m)
		{
			this->showStatusMessage(m);
		});

		auto list = chain->getMainController()->getActiveFileHandler()->pool->getSampleMapPool().getIdList();
		list.insert(0, "Select sample map");

		row1 = new AdditionalRow(this);

		auto rb = new HiseShapeButton("refresh", nullptr, *this);

		row1->addCustomComponent(rb);
		rb->onClick = [&]()
		{
			runTask(BIND_MEMBER_FUNCTION_0(WavetableConverterDialog::rescan), true);
		};

		row1->addComboBox("samplemap", list, "Samplemap", 190);
		row1->getComponent<ComboBox>("samplemap")->addListener(this);
		row1->setInfoTextForLastComponent("The samplemap you want to convert into a .hwt wavetable");

		row1->addComboBox("mode", { "Resample", "Zero Phase", "Static Phase", "Dynamic Phase" }, "Analysis Mode", 120);
		row1->getComponent<ComboBox>("mode")->addListener(this);
		row1->setInfoTextForLastComponent(R"(## Analysis Mode
This setting will define how the wavetable conversion will create the wavetable data and is maybe the most important setting of this dialogue.
### Resample
Use this with audio files that are already in perfect power of two cycle length(like most "wavetable bank" files ready to be dropped in a wavetable synth).This will not perform any resynthesis but just store the cycles into a mip map(with some band limiting applied to higher notes to avoid aliasing).If you can use this mode with your source material, then it's highly recommended to do so as it will avoid the artifacts of any of the resynthesis modes.
### Zero Phase
This will analyse the harmonics of each slice and create a wavetable where every phase is zero.This might be a suitable mode for simple waveforms(sine, saws, etc), but it will mess with the stereo image of stereo wavetables and sound rather static.
### Static Phase
This will analyse the harmonics of each slice and pick a phase from somewhere in the middle of the sample to create a wavetable with varying phase offsets for each harmonic.The resulting wavetable will keep some of the stereo field and might look better than a zero phase resynthesis and is the preferred way of converting samples of synthesiser waveforms into a wavetable.
### Dynamic Phase
This will analyse the sample and recreate every cycle with the correct phase information.This will use the Loris library to resynthesise and pitch lock the sample and export it as wavetable.This might vastly increase the wavetable size, but yields the best results for organic material like real world instruments with subtle pitch differences and a complex stereo image.)");


		

		row1->addCustomComponent(new PreviewButton("Original"));
		row1->addCustomComponent(new PreviewButton("Preview"));

		row1->getComponent<Button>("Original")->addListener(&bl_);
		row1->getComponent<Button>("Preview")->addListener(&bl_);

		soundProperty = new MarkdownHelpButton();

		soundProperty->setHelpText(WavetableHelp::About());

		row1->addCustomComponent(soundProperty);



		row1->setSize(768, 40);

		addCustomComponent(row1);

		sm = new SampleMapToWavetableConverter::SampleMapPreview(*converter);
		sm->setSize(768, 80);

		addCustomComponent(sm);

		row2 = new AdditionalRow(this);

		row2->addCustomComponent(new Separator("Analysis"));

		row2->addComboBox("sourcelength", { "Automatic", "128", "256", "512", "1024", "2048", "4096" }, "Source Length", 92);
		row2->getComponent<ComboBox>("sourcelength")->addListener(this);
		row2->setInfoTextForLastComponent("Use this to define the wavetable length in the original wavefile (Automatic is trying to find the correct length using auto-corellation)  \n> This setting is only used by the **Resample** mode, if you're resynthesizing the sample it will detect the cycle length from the root frequency.");

		row2->addComboBox("offset", { "0", "10%", "25%", "50%", "66%", "75%", "100%" }, "Transients", 82);
		row2->setInfoTextForLastComponent(WavetableHelp::Offset());

		row2->getComponent<ComboBox>("offset")->setSelectedItemIndex(3, dontSendNotification);

		row2->addComboBox("numSlices", { "1", "2", "8", "16", "32", "64", "128", "256" }, "Slices", 60);
		row2->setInfoTextForLastComponent(WavetableHelp::Slices());
		row2->getComponent<ComboBox>("numSlices")->setSelectedItemIndex(5, dontSendNotification);

		row2->addCustomComponent(new Separator("Export"));

		row2->addComboBox("mipmap", { "Octave", "Half Octave", "Semitone", "Chromatic" }, "Mipmap"
			, 92);
		row2->setInfoTextForLastComponent(WavetableHelp::MipmapSize());

		row2->addComboBox("compression", { "No compression", "FLAC codec" }, "Compression", 92);
		row2->setInfoTextForLastComponent("### Compression\nUsing the FLAC codec might decrease the file size on disk but increase the load time of wavetables");

		row2->addComboBox("ReverseTables", { "No", "Yes" }, "Reverse", 72);
		row2->setInfoTextForLastComponent(WavetableHelp::ReverseWavetables());

		row2->addComboBox("Noise", { "Mute", "Mix", "Solo" }, "Residual Noise", 100);

		row2->setInfoTextForLastComponent(R"(### Residual Noise
This will use Loris to separate the noise from the sinusoidal parts of the sample. The preview (and samplemap export) will then either mix or solo the noise signal so you can compare it against the original better.  
> Setting the noise to solo is also a good way to check the quality of the analysis as it gives you hints what went wrong.)");

		row2->setSize(768, 40);
		addCustomComponent(row2);

		preview = new CombinedPreview(*converter, chain->getMainController());
		preview->setSize(768, 300);

		sm->indexBroadcaster.addListener(*this, updateIndex, false);

		auto c = converter.get();

		sm->sampleMapLoadFunction = BIND_MEMBER_FUNCTION_1(WavetableConverterDialog::loadSampleMap);

		preview->waterfall->displayDataFunction = [c, chain_]()
		{
			auto mc = chain_->getMainController();
			WaterfallComponent::DisplayData d;

			d.sound = dynamic_cast<WavetableSound*>(c->sound.get());

			auto previewSize = mc->getPreviewBufferSize();

			if (previewSize != 0)
				d.modValue = (float)mc->getPreviewBufferPosition() / (float)previewSize;
			else
				d.modValue = 0.0f;

			return d;
		};

		addCustomComponent(preview);

		converter->spectrumBroadcaster.addListener(*preview.get(), [](CombinedPreview& p, Image* i)
		{
			if (i != nullptr)
				p.setImageToShow(*i);
		});

		addBasicComponents(false);

		addButton("Export as HWT", 1, KeyPress('h'));
		addButton("Export as Samplemap", 2, KeyPress('s'));

		showStatusMessage("Choose Samplemap to convert");

		setDestroyWhenFinished(false);

		getButton("Export as HWT")->onClick = [&]()
		{
			converter->exportAsHwt = true;
			runTask(BIND_MEMBER_FUNCTION_0(WavetableConverterDialog::buildAllWavetables), true);
		};

		getButton("Export as Samplemap")->onClick = [&]()
		{
			converter->exportAsHwt = false;
			runTask(BIND_MEMBER_FUNCTION_0(WavetableConverterDialog::buildAllWavetables), true);
		};

		preview->waterfall->setColour(HiseColourScheme::ColourIds::ComponentBackgroundColour, Colours::black);
		preview->waterfall->setColour(HiseColourScheme::ColourIds::ComponentFillTopColourId, Colours::white);
		preview->waterfall->setColour(HiseColourScheme::ColourIds::ComponentOutlineColourId, Colours::white.withAlpha(0.5f));
		preview->waterfall->setColour(HiseColourScheme::ColourIds::ComponentTextColourId, Colours::white.withAlpha(0.5f));

		refreshEnablement();
	}

	MarkdownHelpButton* soundProperty;

	struct bl : public ButtonListener
	{
		bl(WavetableConverterDialog& parent_) :
			parent(parent_)
		{};

		~bl()
		{
			parent.chain->getMainController()->setBufferToPlay({}, {});
		}

		int lastPreview = -1;
		WavetableConverterDialog&parent;

		void previewUpdate(int pos)
		{
			auto normValue = (double)pos / (double)(jmax(1, parent.chain->getMainController()->getPreviewBufferSize()));

			if (parent.row1 != nullptr)
			{
				parent.row1->getComponent<PreviewButton>("Original")->setProgress(normValue);
				parent.row1->getComponent<PreviewButton>("Preview")->setProgress(normValue);
			}
		}

		void onPreview()
		{
			parent.chain->getMainController()->stopBufferToPlay();

			auto b = parent.converter->getPreviewBuffers(lastPreview);

			

			parent.wait(50);
			parent.chain->getMainController()->setBufferToPlay(b, parent.converter->sampleRate, BIND_MEMBER_FUNCTION_1(bl::previewUpdate));
		}

		void buttonClicked(Button* b) override
		{
			auto original = b->getName() == "Original";

			parent.row1->getComponent<Button>("Original")->setToggleState(false, dontSendNotification);
			parent.row1->getComponent<Button>("Preview")->setToggleState(false, dontSendNotification);
			b->setToggleState(true, dontSendNotification);

			if (lastPreview != (int)original)
			{
				lastPreview = (int)original;
				parent.converter->originalSpectrum = {};
			}

			parent.chain->getMainController()->stopBufferToPlay();

			parent.runTask(BIND_MEMBER_FUNCTION_0(bl::onPreview), false);
		}
	} bl_;

	static void updateIndex(WavetableConverterDialog& d, int newIndex)
	{
		auto converter = d.converter.get();

		d.runTask([newIndex, converter]()
		{
			converter->setCurrentIndex(newIndex, sendNotificationSync);
		}, true);
	}

	Path createPath(const String& url) const override
	{
		Path path;
		if (url == "refresh")
		{
			static const unsigned char pathData[] = { 110,109,68,196,21,68,162,38,190,67,98,112,28,26,68,227,203,198,67,168,254,31,68,9,170,203,67,208,34,38,68,9,170,203,67,98,196,243,50,68,9,170,203,67,0,88,61,68,148,225,182,67,0,88,61,68,220,62,157,67,108,0,128,69,68,220,62,157,67,98,0,128,69,68,170,227,
	191,67,136,116,55,68,237,248,219,67,208,34,38,68,237,248,219,67,98,148,212,29,68,237,248,219,67,112,223,21,68,116,98,213,67,32,0,16,68,238,174,201,67,108,240,163,8,68,78,103,216,67,108,144,134,8,68,64,107,176,67,108,152,132,28,68,0,166,176,67,108,68,
	196,21,68,162,38,190,67,99,109,188,59,55,68,189,178,123,67,98,36,227,50,68,60,104,106,67,16,1,45,68,124,172,96,67,8,221,38,68,124,172,96,67,98,172,11,26,68,124,172,96,67,152,167,15,68,108,30,133,67,152,167,15,68,36,193,158,67,108,0,128,7,68,36,193,158,
	67,98,0,128,7,68,172,56,120,67,200,138,21,68,37,14,64,67,8,221,38,68,37,14,64,67,98,72,43,47,68,37,14,64,67,4,32,55,68,23,59,77,67,80,255,60,68,36,162,100,67,108,164,91,68,68,100,49,71,67,108,192,120,68,68,192,148,139,67,108,244,123,48,68,0,90,139,67,
	108,188,59,55,68,189,178,123,67,99,101,0,0 };
			path.loadPathFromData(pathData, sizeof(pathData));
		}

		return path;
	}

	void runTask(const std::function<void(void)>& f, bool clearTasks=true)
	{
		rebuildPending = true;

		auto isRunning = getCurrentThread() != nullptr && getCurrentThread()->isThreadRunning();

		if (clearTasks)
		{
			if (isRunning)
			{
				if (getCurrentThread()->stopThread(1000))
					isRunning = false;
			}

			currentTasks.callForEveryElementInQueue([](std::function<void(void)>& f) {return true; });
		}

		currentTasks.push(f);

		if (!isRunning)
		{
			runThread();
		}
	}

	std::function<void(void)> finishFunction;

	LockfreeQueue<std::function<void(void)>> currentTasks;

	~WavetableConverterDialog()
	{
		fileHandling = nullptr;
		preview = nullptr;
		converter = nullptr;
	}

	void refreshEnablement()
	{
		auto resynthesise = converter->phaseMode != SampleMapToWavetableConverter::PhaseMode::Resample;

		row2->getComponent<ComboBox>("offset")->setEnabled(resynthesise);
		row2->getComponent<ComboBox>("numSlices")->setEnabled(resynthesise && converter->phaseMode != SampleMapToWavetableConverter::PhaseMode::DynamicPhase);
		row2->getComponent<ComboBox>("sourcelength")->setEnabled(!resynthesise);

		row2->getComponent<ComboBox>("Noise")->setEnabled(resynthesise);

		getButton("Export as Samplemap")->setEnabled(resynthesise);
	}

	void rescan()
	{
		showStatusMessage("Rebuilding current sample");
		converter->refreshCurrentWavetable(true);
		showStatusMessage("Rebuilding done");
		
		refreshPreview();
	}

	void refreshPreview()
	{
		converter->sendChangeMessage();
	}

	void cancelCurrentTask()
	{
		if (auto ct = getCurrentThread())
			ct->stopThread(1000);
	}

	void loadSampleMap(const ValueTree& v)
	{
		ValueTree copy(v);

		runTask([this, copy]()
		{
			currentlyLoadedMap = copy[SampleIds::ID].toString();

			converter->parseSampleMap(copy);
			
			showStatusMessage("Loaded map " + currentlyLoadedMap);
			refreshPreview();
		}, true);
	}

	void comboBoxChanged(ComboBox* comboBoxThatHasChanged) override
	{
		if (comboBoxThatHasChanged->getName() == "mode")
		{
			cancelCurrentTask();

			converter->phaseMode = (SampleMapToWavetableConverter::PhaseMode)comboBoxThatHasChanged->getSelectedItemIndex();

			refreshEnablement();
			
			converter->discardAllScans();

			runTask(BIND_MEMBER_FUNCTION_0(WavetableConverterDialog::rescan));
		}
		if (comboBoxThatHasChanged->getName() == "compression")
		{
			converter->useCompression = comboBoxThatHasChanged->getSelectedItemIndex();
			return;
		}
		if (comboBoxThatHasChanged->getName() == "samplemap")
		{
			if (comboBoxThatHasChanged->getSelectedItemIndex() == 0)
				return;

			auto& spool = chain->getMainController()->getActiveFileHandler()->pool->getSampleMapPool();

			PoolReference ref(chain->getMainController(), comboBoxThatHasChanged->getText(), FileHandlerBase::SampleMaps);

			if (auto vData = spool.loadFromReference(ref, PoolHelpers::LoadAndCacheWeak))
			{
				loadSampleMap(vData->data);
			}

			return;
		}
		if (comboBoxThatHasChanged->getName() == "Noise")
		{
			auto nm = (SampleMapToWavetableConverter::PreviewNoise)comboBoxThatHasChanged->getSelectedItemIndex();

			runTask([this, nm]()
			{
				converter->setPreviewMode(nm);
				row1->getComponent<Button>("Preview")->triggerClick(sendNotificationAsync);
			});
			
			return;
		}
		if (comboBoxThatHasChanged->getName() == "mipmap")
		{
			cancelCurrentTask();

			static const int items[4] = { 12, 6, 2, 1 };
			converter->mipmapSize = items[comboBoxThatHasChanged->getSelectedItemIndex()];
		}
        if (comboBoxThatHasChanged->getName() == "sourcelength")
		{
			cancelCurrentTask();

			auto cl = comboBoxThatHasChanged->getSelectedItemIndex();

			if (cl >= 1)
				cl = std::pow(2, 6 + cl);

			converter->cycleLength = cl;
			converter->discardAllScans();
			
		}
        if (comboBoxThatHasChanged->getName() == "numSlices")
        {
			cancelCurrentTask();

            converter->numParts = comboBoxThatHasChanged->getText().getIntValue();
			runTask(BIND_MEMBER_FUNCTION_0(WavetableConverterDialog::rescan));
        }
        if (comboBoxThatHasChanged->getName() == "offset")
        {
            static const double items[7] = {0.0, 0.1, 0.25, 0.5, 0.66666, 0.75, 1.0 };

            converter->offsetInSlice = items[comboBoxThatHasChanged->getSelectedItemIndex()];
			converter->discardAllScans();

			runTask(BIND_MEMBER_FUNCTION_0(WavetableConverterDialog::rescan));
        }
		else if (comboBoxThatHasChanged->getName() == "ReverseTables")
		{
			cancelCurrentTask();

			converter->reverseOrder = comboBoxThatHasChanged->getSelectedItemIndex() == 1;
		}
        
		refreshPreview();
	}

	void buildAllWavetables()
	{
		converter->exportAll();

		if (converter->phaseMode == SampleMapToWavetableConverter::PhaseMode::Resample)
		{
			showStatusMessage("Wavetables exported with " + String(converter->cycleLength) + " cycle length");
		}

		if (threadShouldExit())
			return;

		auto exportedTree = converter->getValueTree();

		auto outputFileName = currentlyLoadedMap;

		if (converter->exportAsHwt)
			outputFileName << ".hwt";
		else
			outputFileName << converter->getPrefixFromNoiseMode(-1) << ".xml";

		currentFile = GET_PROJECT_HANDLER(chain).getSubDirectory(converter->exportAsHwt ? FileHandlerBase::AudioFiles : FileHandlerBase::SampleMaps).getChildFile(outputFileName);

		if (converter->exportAsHwt)
		{
			PresetHandler::writeValueTreeAsFile(exportedTree, currentFile.getFullPathName());
		}
		else
		{
			currentFile.replaceWithText(exportedTree.createXml()->createDocument(""));

			chain->getMainController()->getCurrentSampleMapPool()->refreshPoolAfterUpdate();
		}

		done = true;
	}

	void run() override
	{
		getProgressCounter() = 0.0;

		r = Result::ok();

		lastTime = 0;
		converter->threadController = new ThreadController(getCurrentThread(), &getProgressCounter(), 1000, lastTime);

		int currentIndex = 0;
		int numTotal = currentTasks.size();

		currentTasks.callForEveryElementInQueue([&](std::function<void(void)>& f)
		{
			if (auto s = ThreadController::ScopedStepScaler(converter->threadController.get(), currentIndex++, numTotal))
			{
				try
				{
					f();
				}				
				catch (Result& r)
				{
					showStatusMessage(r.getErrorMessage());
					return true;
				}
				
				return true;
			}
			else
			{
				return false;
			}

		});

		getProgressCounter() = 1.0;

		if (r.failed())
			showStatusMessage(r.getErrorMessage());

		rebuildPending = false;
		
	}

	void threadFinished() override
	{
		stopThread();

		if (auto s = dynamic_cast<WavetableSound*>(converter->sound.get()))
		{
			soundProperty->setHelpText(s->getMarkdownDescription());
		}

		if (done)
		{
			PresetHandler::showMessageWindow("Conversion OK", "Wavetable saved to " + currentFile.getFileName());
			done = false;
		}
	}

	uint32_t lastTime = 0;
	bool done = false;
	double wavetableProgress = 0.0;
	int numWavetables = 0;

	Result r;

	ScopedPointer<CombinedPreview> preview;
	ScopedPointer<SampleMapToWavetableConverter::SampleMapPreview> sm;

    bool rebuildPending = false;
	

	ModulatorSynthChain* chain;
	
	ScopedPointer<AdditionalRow> fileHandling;
	ScopedPointer<AdditionalRow> row1;
	ScopedPointer<AdditionalRow> row2;
	ScopedPointer<SampleMapToWavetableConverter> converter;

	String currentlyLoadedMap;

	File currentFile;


	JUCE_DECLARE_WEAK_REFERENCEABLE(WavetableConverterDialog);
};


class MonolithConverter : public MonolithExporter
{
public:

	enum ExportOptions
	{
		ExportAll,
		ExportSampleMapsAndJSON,
		ExportJSON,
		numExportOptions
	};

	MonolithConverter(BackendRootWindow* bpe_) :
		MonolithExporter("Convert samples to Monolith + Samplemap", bpe_->getMainSynthChain()),
		bpe(bpe_),
		chain(bpe_->getMainSynthChain())
	{
		sampler = dynamic_cast<ModulatorSampler*>(ProcessorHelpers::getFirstProcessorWithName(chain, "Sampler"));
		sampleFolder = GET_PROJECT_HANDLER(chain).getSubDirectory(ProjectHandler::SubDirectories::Samples);

		jassert(sampler != nullptr);

		StringArray directoryDepth;
		directoryDepth.add("1");
		directoryDepth.add("2");
		directoryDepth.add("3");
		directoryDepth.add("4");

		addComboBox("directoryDepth", directoryDepth, "Directory Depth");

		StringArray yesNo;

		yesNo.add("Yes");
		yesNo.add("No"); // best code I've ever written...

		addComboBox("overwriteFiles", yesNo, "Overwrite existing files");
		
		StringArray option;
		option.add("Export all");
		option.add("Skip monolith file creation");
		option.add("Only create JSON data file");

		addComboBox("option", option, "Export Depth");

		addTextEditor("directorySeparator", "::", "Directory separation character");

		StringArray sa2;

		sa2.add("No normalisation");
		sa2.add("Normalise every sample");
		sa2.add("Full Dynamics");

		addComboBox("normalise", sa2, "Normalization");

		addBasicComponents(true);
	};

	void getDepth(const File& rootFile, const File& childFile, int& counter)
	{
		jassert(childFile.isAChildOf(rootFile));

		if (childFile.getParentDirectory() == rootFile)
		{
			return;
		}

		counter++;

		getDepth(rootFile, childFile.getParentDirectory(), counter);
	}

	void convertSampleMap(const File& sampleDirectory, bool overwriteExistingData, bool exportSamples, bool exportSampleMap)
	{
		if (!exportSamples && !exportSampleMap) return;

#if JUCE_WINDOWS
		const String slash = "\\";
#else
		const String slash = "/";
#endif

		const String sampleMapPath = sampleDirectory.getRelativePathFrom(sampleFolder);
		const String sampleMapId = sampleMapPath.replace(slash, "_");

		showStatusMessage("Importing " + sampleMapId);

		Array<File> samples;

		sampleDirectory.findChildFiles(samples, File::findFiles, true, "*.wav;*.aif;*.aif;*.WAV;*.AIF;*.AIFF");

		StringArray fileNames;

		for (int i = 0; i < samples.size(); i++)
		{
			if (samples[i].isHidden() || samples[i].getFileName().startsWith("."))
				continue;

			fileNames.add(samples[i].getFullPathName());
		}

        auto& tmpBpe = bpe;
		
        StringArray fileNamesCopy;
        
        fileNamesCopy.addArray(fileNames);
        
        auto f = [tmpBpe, fileNamesCopy](Processor* p)
        {
            if(auto sampler = dynamic_cast<ModulatorSampler*>(p))
            {
                sampler->clearSampleMap(dontSendNotification);
                SampleImporter::loadAudioFilesRaw(tmpBpe, sampler, fileNamesCopy);
                SampleEditHandler::SampleEditingActions::automapUsingMetadata(sampler);
            }
            
            return SafeFunctionCall::Status::OK;
        };
        
        sampler->killAllVoicesAndCall(f);
		
        Thread::sleep(500);
        
		sampler->getSampleMap()->setId(sampleMapId);
		sampler->getSampleMap()->setIsMonolith();

		setSampleMap(sampler->getSampleMap());

        auto sampleMapFolder = GET_PROJECT_HANDLER(chain).getSubDirectory(ProjectHandler::SubDirectories::SampleMaps);
        
		sampleMapFile = sampleMapFolder.getChildFile(sampleMapPath + ".xml");

        //sampleMapFile = sampleMapFolder.getChildFile(sampleMapId + ".xml");
        
		auto& lock = sampler->getMainController()->getSampleManager().getSampleLock();

		while (!lock.tryEnter())
			Thread::sleep(500);

		lock.exit();

		exportCurrentSampleMap(overwriteExistingData, exportSamples, exportSampleMap);
	}

	void run() override
	{		
		generateDirectoryList();

		showStatusMessage("Writing JSON list");

		const String data = JSON::toString(fileNameList);
		File f = GET_PROJECT_HANDLER(bpe->getMainSynthChain()).getSubDirectory(ProjectHandler::SubDirectories::Scripts).getChildFile("samplemaps.js");
		f.replaceWithText(data);

		
        ExportOptions optionIndex = (ExportOptions)getComboBoxComponent("option")->getSelectedItemIndex();
        
        const bool exportSamples = (optionIndex == ExportAll);
        const bool exportSampleMap = (optionIndex == ExportSampleMapsAndJSON) || (optionIndex == ExportAll);
        const bool overwriteData = getComboBoxComponent("overwriteFiles")->getSelectedItemIndex() == 0;
        
        
        
        for (int i = 0; i < fileList.size(); i++)
        {
            if (threadShouldExit())
            {
                break;
            }
            
            setProgress((double)i / (double)fileList.size());
            convertSampleMap(fileList[i], overwriteData, exportSamples, exportSampleMap);
        }
	}

	void generateDirectoryList()
	{
		sampler->getMainController()->getSampleManager().getModulatorSamplerSoundPool()->setUpdatePool(false);

		Array<File> allDirectories;

		const int depth = getComboBoxComponent("directoryDepth")->getText().getIntValue();


		sampleFolder.findChildFiles(allDirectories, File::findDirectories, true);
		const String separator = getTextEditor("directorySeparator")->getText();

		for (int i = 0; i < allDirectories.size(); i++)
		{
			int thisDepth = 1;

			getDepth(sampleFolder, allDirectories[i], thisDepth);

			if (thisDepth == depth)
			{
				fileList.add(allDirectories[i]);
			}
		}

		for (int i = 0; i < fileList.size(); i++)
		{

#if JUCE_WINDOWS
			const String slash = "\\";
#else
			const String slash = "/";
#endif

			String s = fileList[i].getRelativePathFrom(sampleFolder).replace(slash, separator);

			fileNameList.add(var(s));

		}
	}

	void threadFinished() override
	{
        
	};

private:

	

	Array<var> fileNameList;
	Array<File> fileList;

	File sampleFolder;
	BackendRootWindow* bpe;
	ModulatorSampler* sampler;
	ModulatorSynthChain* chain;
};



class WavetableMonolithExporter : public DialogWindowWithBackgroundThread,
								  public ControlledObject
{
public:

	WavetableMonolithExporter(MainController* mc) :
		DialogWindowWithBackgroundThread("Exporting wavetable banks"),
		ControlledObject(mc)
	{
		auto expList = mc->getExpansionHandler().getListOfAvailableExpansions();

		StringArray expansionList;
		expansionList.add("Factory Content");

		for (int i = 0; i < expList.size(); i++)
			expansionList.add(expList[i].toString());

		addComboBox("expansion", expansionList, "Expansion");

		addBasicComponents(true);
	}

	void run() override
	{
		showStatusMessage("Exporting wavetables");

		auto audioFileFolder = getMainController()->getCurrentFileHandler().getSubDirectory(FileHandlerBase::AudioFiles);
		auto sampleFolder = getMainController()->getCurrentFileHandler().getSubDirectory(FileHandlerBase::Samples);

		if (auto e = getMainController()->getExpansionHandler().getExpansionFromName(getComboBoxComponent("expansion")->getText()))
		{
			audioFileFolder = e->getSubDirectory(FileHandlerBase::AudioFiles);
			sampleFolder = e->getSubDirectory(FileHandlerBase::Samples);
		}

		targetFile = sampleFolder.getChildFile("wavetables.hwm");

		auto wavetableFiles = audioFileFolder.findChildFiles(File::findFiles, false, "*.hwt");

		wavetableFiles.sort();

		size_t numBytes = 0;

		for (auto wv : wavetableFiles)
			numBytes += wv.getSize(); 

		MemoryOutputStream mos;
		mos.preallocate(numBytes);

		int i = 0;

		MemoryOutputStream headerOutput;

		auto projectName = GET_HISE_SETTING(getMainController()->getMainSynthChain(), HiseSettings::Project::Name).toString();

		auto encryptionKey = GET_HISE_SETTING(getMainController()->getMainSynthChain(), HiseSettings::Project::EncryptionKey).toString();

		WavetableMonolithHeader::writeProjectInfo(headerOutput, projectName, encryptionKey);

		

		
		

		for (auto wv : wavetableFiles)
		{
			FileInputStream fis(wv);

			WavetableMonolithHeader headerChild;
			headerChild.name = wv.getFileNameWithoutExtension();
			headerChild.offset = mos.getPosition();

			setProgress((double)(i++) / (double)wavetableFiles.size());

			headerChild.write(headerOutput);
			mos.writeFromInputStream(fis, fis.getTotalLength());
		}

		showStatusMessage("Writing output file");

		mos.flush();

		if (targetFile.existsAsFile())
			targetFile.deleteFile();

		FileOutputStream fos(targetFile);

		headerOutput.flush();

		fos.writeInt64((int64)headerOutput.getDataSize());
		fos.write(headerOutput.getData(), headerOutput.getDataSize());

		fos.writeInt64((int64)mos.getDataSize());
		fos.write(mos.getData(), mos.getDataSize());

		fos.flush();
	}

	File targetFile;

	void threadFinished() override
	{
		PresetHandler::showMessageWindow("Export successful", "All wavetables have been exported to " + targetFile.getFullPathName());
	}
};

class PoolExporter : public DialogWindowWithBackgroundThread
{
public:

	PoolExporter(MainController* mc_):
		DialogWindowWithBackgroundThread("Exporting pool resources"),
		mc(mc_)
	{
		addBasicComponents(false);

		runThread();
	}

	void run() override
	{
		showStatusMessage("Exporting pools");

		auto& handler = mc->getCurrentFileHandler();
		handler.exportAllPoolsToTemporaryDirectory(mc->getMainSynthChain(), &logData);
	}

	void threadFinished() override
	{
		PresetHandler::showMessageWindow("Sucessfully exported", "All pools were successfully exported");

		auto& data = dynamic_cast<GlobalSettingManager*>(mc)->getSettingsObject();
		auto project = data.getSetting(HiseSettings::Project::Name).toString();
		auto company = data.getSetting(HiseSettings::User::Company).toString();
		auto targetDirectory = ProjectHandler::getAppDataDirectory(nullptr).getParentDirectory().getChildFile(company).getChildFile(project);
		auto& handler = mc->getCurrentFileHandler();

		if (!(bool)data.getSetting(HiseSettings::Project::EmbedImageFiles))
		{
			if (PresetHandler::showYesNoWindow("Copy image pool file to App data directory",
				"Do you want to copy the ImageResources.dat file to the app data directory?\nThis is required for the compiled plugin to load the new resources on this machine"))
			{
				auto f = handler.getTempFileForPool(FileHandlerBase::Images);
				jassert(f.existsAsFile());
				f.copyFileTo(targetDirectory.getChildFile(f.getFileName()));
			}
		}

		if (!(bool)data.getSetting(HiseSettings::Project::EmbedAudioFiles))
		{
			if (PresetHandler::showYesNoWindow("Copy audio pool file to App data directory",
				"Do you want to copy the AudioResources.dat file to the app data directory?\nThis is required for the compiled plugin to load the new resources on this machine"))
			{
				auto f = handler.getTempFileForPool(FileHandlerBase::AudioFiles);
				jassert(f.existsAsFile());
				f.copyFileTo(targetDirectory.getChildFile(f.getFileName()));
			}
		}
	}

private:

	MainController* mc;
};


class CyclicReferenceChecker: public DialogWindowWithBackgroundThread
{
public:

	CyclicReferenceChecker(BackendProcessorEditor* bpe_) :
		DialogWindowWithBackgroundThread("Checking cyclic references"),
		bpe(bpe_)
	{
		StringArray processorList = ProcessorHelpers::getAllIdsForType<JavascriptProcessor>(bpe->getMainSynthChain());

		addComboBox("scriptProcessor", processorList, "ScriptProcessor to analyze");
		addBasicComponents(true);
	}

	void run() override
	{
		
		setProgress(-1.0);
		
		auto id = getComboBoxComponent("scriptProcessor")->getText();
		auto jsp = dynamic_cast<JavascriptProcessor*>(ProcessorHelpers::getFirstProcessorWithName(bpe->getMainSynthChain(), id));

		if (jsp != nullptr)
		{
			
			data.progress = &logData.progress;
			data.thread = this;
			
			showStatusMessage("Recompiling script");

			{
				// don't bother...
				MessageManagerLock mm;

				jsp->compileScriptWithCycleReferenceCheckEnabled();
			}

			showStatusMessage("Checking Cyclic references");

			jsp->getScriptEngine()->checkCyclicReferences(data, Identifier());

		}

	}

	void threadFinished()
	{
		if (data.overflowHit)
		{
			PresetHandler::showMessageWindow("Overflow", "The reference check was cancelled due to a stack overflow", PresetHandler::IconType::Error);
		}
		else if (data.cyclicReferenceString.isNotEmpty())
		{
			if (PresetHandler::showYesNoWindow("Cyclic References found", "The " + data.cyclicReferenceString + " is a cyclic reference. Your script will be leaking memory.\nPres OK to copy this message to the clipboard.",
				PresetHandler::IconType::Error))
			{
				SystemClipboard::copyTextToClipboard(data.cyclicReferenceString);
			}
		}
		else
		{
			PresetHandler::showMessageWindow("No Cyclic References found", 
										     "Your script does not contain cyclic references.\n" + String(data.numChecked) + " references were checked", 
											 PresetHandler::IconType::Info);
		}
	}

private:

	HiseJavascriptEngine::CyclicReferenceCheckBase::ThreadData data;


	BackendProcessorEditor* bpe;
};

class RNBOTemplateBuilder: public DialogWindowWithBackgroundThread
{
public:

    RNBOTemplateBuilder(BackendRootWindow* bpe_) :
        DialogWindowWithBackgroundThread("Create RNBO Template files"),
        bpe(bpe_),
        config(new AdditionalRow(this)),
        dataTypes(new AdditionalRow(this))
    {
        StringArray sa;
        
        root = BackendDllManager::getRNBOSourceFolder(bpe->getMainController()).getParentDirectory();
        
        auto cppFiles = root.findChildFiles(File::findFiles, false, "*.cpp");
        
        for(auto f: cppFiles)
        {
            sa.add(f.getFileName());
        }
        
        
        
        config->addComboBox("rnbo_file", sa, "RNBO Patch");
        config->setInfoTextForLastComponent("The RNBO patch you want to create the wrapper for.  > If this is empty, make sure you have exported the RNBO patch to the correct directory: `" + root.getFullPathName() + "`");
        
        config->addComboBox("polyphony", {"Disabled", "Enabled"}, "Polyphony");
        config->setInfoTextForLastComponent("Enables the polyphonic use of the RNBO patch. Please be aware that you always need to export the RNBO patch with the polyphony setting **Disabled** and then enable it here");
        
        config->addComboBox("use_mod", {"Disabled", "Enabled"}, "Modulation Output");
        config->setInfoTextForLastComponent("Adds a modulation output to the node. Use this if you want to create a draggable modulation source for this node. If you enable this, you will have to send out a signal to the outport with the ID `modOutput`");
        
        config->addComboBox("use_tempo", {"Disabled", "Enabled"}, "Tempo Sync");
        config->setInfoTextForLastComponent("Registers this node to receive tempo events. Enable this if the RNBO patch requires tempo syncing");
        
        config->addTextEditor("num_channels", "2", "Channel Amount");
        config->setInfoTextForLastComponent("The number of audio channels that this node is using. Set this to the number of `out~` ports of the RNBO patch");
        
        dataTypes->addTextEditor("table_ids", "", "Table IDs");
        dataTypes->setInfoTextForLastComponent("A comma-separated list of all buffer IDs that you want to show as table in HISE");
        dataTypes->addTextEditor("slider_pack_ids", "", "SliderPack IDs");
        dataTypes->setInfoTextForLastComponent("A comma-separated list of all buffer IDs that you want to show as slider pack in HISE");
        dataTypes->addTextEditor("audio_file_ids", "", "AudioFile IDs");
        dataTypes->setInfoTextForLastComponent("A comma-separated list of all buffer IDs that you want to show as audio file in HISE  > Be aware that it currently only supports loading a single audio file");
        
        config->setSize(512, 40);
        dataTypes->setSize(512, 40);
        
        addCustomComponent(config);
        addCustomComponent(dataTypes);
        
        addBasicComponents(true);
        
        if(sa.isEmpty())
            showStatusMessage("No RNBO Files found");
        else
            showStatusMessage("Press OK to create a C++ template file");
    }

    ScopedPointer<AdditionalRow> config;
    ScopedPointer<AdditionalRow> dataTypes;
    
    void run() override
    {
        auto numChannels = config->getComponent<TextEditor>("num_channels")->getText().getIntValue();
        
        
        
        auto tableIds = StringArray::fromTokens(dataTypes->getComponent<TextEditor>("table_ids")->getText(), ",", "");
        auto sliderPackIds = StringArray::fromTokens(dataTypes->getComponent<TextEditor>("slider_pack_ids")->getText(), ",", "");
        auto audioFileIds = StringArray::fromTokens(dataTypes->getComponent<TextEditor>("audio_file_ids")->getText(), ",", "");
        
        tableIds.removeEmptyStrings();
        sliderPackIds.removeEmptyStrings();
        audioFileIds.removeEmptyStrings();
        
        auto allowPoly = (bool)config->getComponent<ComboBox>("polyphony")->getSelectedItemIndex();
        auto useMod = (bool)config->getComponent<ComboBox>("use_mod")->getSelectedItemIndex();
        auto useTempo = (bool)config->getComponent<ComboBox>("use_tempo")->getSelectedItemIndex();
         
        auto rnboFile = root.getChildFile(config->getComponent<ComboBox>("rnbo_file")->getText());
        
        using namespace snex::cppgen;
        
        
        auto thirdPartyFolder = BackendDllManager::getSubFolder(bpe->getMainController(), BackendDllManager::FolderSubType::ThirdParty);
        
        Base b(Base::OutputType::AddTabs);
        
        b.setHeader([](){return "/* Autogenerated template file for RNBO exported class. */";});
        
        b << "#pragma once";
        b.addEmptyLine();
        Include(b, "hi_dsp_library/node_api/nodes/rnbo_wrapper.h");
        Include(b, thirdPartyFolder, rnboFile);
        
        b.addEmptyLine();
        
        classId = rnboFile.getFileNameWithoutExtension();
        
        
        {
            Namespace n(b, "project", false);
            
            UsingNamespace(b, NamespacedIdentifier("scriptnode"));
            
            snex::TemplateParameter templateArg(NamespacedIdentifier("NV"), 0, false);
            
            
            {
                String bc;
                bc << "wrap::" << (useMod ? "rnbo_wrapper_with_mod" : "rnbo_wrapper");
                bc << "<RNBO::" << classId << ", NV>";
                
                auto baseClass = NamespacedIdentifier::fromString(bc);
                
                Struct s1(b, classId, {baseClass}, {templateArg}, true);
                
                String fc;
                
                if(!tableIds.isEmpty())
                {
                    String b_;
                    b_ << "static constexpr int NumTables = " << String(tableIds.size()) << ";";
                    b << b_;
                }
                if(!sliderPackIds.isEmpty())
                {
                    String b_;
                    b_ << "static constexpr int NumSliderPacks = " << String(sliderPackIds.size()) << ";";
                    b << b_;
                }
                if(!audioFileIds.isEmpty())
                {
                    String b_;
                    b_ << "static constexpr int NumAudioFiles = " << String(audioFileIds.size()) << ";";
                    b << b_;
                }
                
                b.addEmptyLine();
                
                b << (classId + String("()"));
                
                {
                    StatementBlock bl(b);
                    
                    if(useTempo)
                        b << "this->setUseTempo(true);";
                    
                    for(auto& t: tableIds)
                    {
                        String l;
                        l << "this->dataHandler.registerDataSlot(ExternalData::DataType::Table";
                        l << ", " << t.trim().quoted() << ");";
                        b << l;
                    }
                    
                    for(auto& t: sliderPackIds)
                    {
                        String l;
                        l << "this->dataHandler.registerDataSlot(ExternalData::DataType::SliderPack";
                        l << ", " << t.trim().quoted() << ");";
                        b << l;
                    }
                    
                    for(auto& t: audioFileIds)
                    {
                        String l;
                        l << "this->dataHandler.registerDataSlot(ExternalData::DataType::AudioFile";
                        l << ", " << t.trim().quoted() << ");";
                        b << l;
                    }
                }
                
                b.addEmptyLine();
                
                fc << "static constexpr int getFixChannelAmount() { return " << String(numChannels) << "; };";
                
                String pc;
                
                pc << "static constexpr bool isPolyphonic() { return ";
                
                if(allowPoly)
                    pc << "NV > 1; };";
                else
                    pc << "false; };";
                
                Macro(b, "SN_GET_SELF_AS_OBJECT", {classId});
                
                b.addEmptyLine();
                
                b << pc;
                b << fc;
                
                b.addEmptyLine();
                
                {
                    Struct s2(b, "MetadataClass", {}, {});
                    Macro(b, "SN_NODE_ID", {classId.quoted()});
                }
            }
        }
        
        auto x = b.toString();
        
        
        
        thirdPartyFolder.getChildFile(classId).withFileExtension(".h").replaceWithText(x);
        
        if(allowPoly)
            BackendDllManager::addNodePropertyToJSONFile(bpe->getMainController(), classId, PropertyIds::IsPolyphonic);
    }

    void threadFinished()
    {
        PresetHandler::showMessageWindow("RNBO File template created", "The file " + classId + " was created. Please reexport the DLL in order to use it in scriptnode");
    }

private:

    String classId;
    BackendRootWindow* bpe;
    File root;
};



struct ShortcutEditor : public QuasiModalComponent,
						public Component,
					    public PathFactory
{
	ShortcutEditor(BackendRootWindow* t) :
		QuasiModalComponent(),
		editor(t->getKeyPressMappingSet(), true),
		closeButton("close", nullptr, *this)
	{
		addAndMakeVisible(editor);
		setName("Edit Shortcuts");
		setSize(600, 700);

		editor.setLookAndFeel(&alaf);
		editor.setColours(Colours::transparentBlack, alaf.bright);
		setLookAndFeel(&alaf);
		addAndMakeVisible(closeButton);

		closeButton.onClick = [&]()
		{
			destroy();
		};
	};

	Path createPath(const String&) const override
	{
		Path p;
		p.loadPathFromData(HiBinaryData::ProcessorEditorHeaderIcons::closeIcon, sizeof(HiBinaryData::ProcessorEditorHeaderIcons::closeIcon));
		return p;
	}

	void resized() override
	{
		auto b = getLocalBounds();
		b.removeFromTop(32);

		closeButton.setBounds(getLocalBounds().removeFromTop(37).removeFromRight(37).reduced(6));
		editor.setBounds(b.reduced(10));
	}

	void mouseDown(const MouseEvent& e)
	{
		dragger.startDraggingComponent(this, e);
	}

	void mouseDrag(const MouseEvent& e)
	{
		dragger.dragComponent(this, e, nullptr);
	}

	juce::ComponentDragger dragger;

	void paint(Graphics& g) override
	{
		ColourGradient grad(alaf.dark.withMultipliedBrightness(1.4f), 0.0f, 0.0f,
			alaf.dark, 0.0f, (float)getHeight(), false);

		auto a = getLocalBounds().removeFromTop(37).toFloat();

		g.setFont(GLOBAL_BOLD_FONT().withHeight(17.0f));
		g.setGradientFill(grad);
		g.fillAll();
		g.setColour(Colours::white.withAlpha(0.1f));
		g.fillRect(a);
		g.setColour(alaf.bright);

		g.drawRect(getLocalBounds().toFloat());

		g.drawText("Edit Shortcuts", a, Justification::centred);
		
		
	}

	HiseShapeButton closeButton;
	AlertWindowLookAndFeel alaf;
	juce::KeyMappingEditorComponent editor;
};

class SampleMapPropertySaverWithBackup : public DialogWindowWithBackgroundThread,
										 public ControlledObject
{
public:

	enum class Preset
	{
		None,
		All,
		Positions,
		Volume,
		Tables,
		numPresets
	};

	static Array<Identifier> getPropertyIds(Preset p)
	{
		switch (p)
		{
		case Preset::None:			return {};
		case Preset::All:			return { SampleIds::GainTable, SampleIds::PitchTable, SampleIds::LowPassTable,
											 SampleIds::SampleStart, SampleIds::SampleEnd, SampleIds::LoopXFade,
											 SampleIds::Volume, SampleIds::Pitch, SampleIds::Normalized };
		case Preset::Positions:		return { SampleIds::SampleStart, SampleIds::SampleEnd, SampleIds::LoopXFade };
		case Preset::Volume:		return { SampleIds::Volume, SampleIds::Pitch, SampleIds::Normalized };
		case Preset::Tables:		return { SampleIds::GainTable, SampleIds::PitchTable, SampleIds::LowPassTable };
		default:					return {};
		}
	}

	struct PropertySelector: public Component,
							 public ComboBoxListener
	{
		struct Item : public Component
		{
			Item(Identifier& id_):
				id(id_.toString())
			{
				setRepaintsOnMouseActivity(true);
			}

			void mouseDown(const MouseEvent& e) override
			{
				active = !active;
				repaint();
			}

			void paint(Graphics& g) override
			{
				auto b = getLocalBounds().toFloat().reduced(1.0f);

				g.setColour(Colours::white.withAlpha(isMouseOver(true) ? 0.3f : 0.2f));
				g.fillRect(b);
				g.drawRect(b, 1.0f);
				g.setColour(Colours::white.withAlpha(active ? 0.9f : 0.2f));
				g.setFont(GLOBAL_MONOSPACE_FONT());
				g.drawText(id, b, Justification::centred);
			}

			String id;
			bool active = false;
		};

		PropertySelector()
		{
			for (auto id : getPropertyIds(Preset::All))
			{
				auto item = new Item(id);
				addAndMakeVisible(item);
				items.add(item);
			}

			addAndMakeVisible(presets);
			presets.addItemList({ "None", "All", "Positions", "Volume", "Tables" }, 1);
			presets.addListener(this);
			presets.setTextWhenNothingSelected("Presets");
			

			setSize(350, 100);
		}

		void comboBoxChanged(ComboBox*) override
		{
			Preset p = (Preset)presets.getSelectedItemIndex();

			auto selectedIds = getPropertyIds(p);

			for (auto i : items)
			{
				i->active = selectedIds.contains(Identifier(i->id));
				i->repaint();
			}
		}

		void paint(Graphics& g) override
		{
			auto b = getLocalBounds().removeFromTop(24).toFloat();
			g.setFont(GLOBAL_BOLD_FONT());
			g.setColour(Colours::white);
			g.drawText("Properties to apply", b, Justification::centredLeft);
		}

		void resized() override
		{
			auto b = getLocalBounds();
			auto top = b.removeFromTop(24);

			

			static constexpr int NumRows = 3;
			static constexpr int NumCols = 3;

			auto rowHeight = b.getHeight() / 3;
			auto colWidth = b.getWidth() / 3;

			int cellIndex = 0;

			presets.setBounds(top.removeFromRight(colWidth));

			for (int row = 0; row < NumRows; row++)
			{
				auto r = b.removeFromTop(rowHeight);

				for (int col = 0; col < NumCols; col++)
				{
					auto cell = r.removeFromLeft(colWidth);
					items[cellIndex++]->setBounds(cell);
				}
			}
		}

		OwnedArray<Item> items;
		ComboBox presets;
	};

	SampleMapPropertySaverWithBackup(BackendRootWindow* bpe) :
		DialogWindowWithBackgroundThread("Apply Samplemap Properties"),
		ControlledObject(bpe->getMainController()),
		result(Result::ok())
	{
		auto samplemapList = getMainController()->getCurrentFileHandler().pool->getSampleMapPool().getIdList();
		addComboBox("samplemapId", samplemapList, "SampleMap");
		addTextEditor("backup_postfix", "_backup", "Backup folder suffix");

		sampleMapId = getComboBoxComponent("samplemapId");
		sampleMapId->onChange = BIND_MEMBER_FUNCTION_0(SampleMapPropertySaverWithBackup::refresh);
		suffix = getTextEditor("backup_postfix");
		suffix->onTextChange = BIND_MEMBER_FUNCTION_0(SampleMapPropertySaverWithBackup::refresh);

		addCustomComponent(propertySelector = new PropertySelector());

		addBasicComponents(true);

		refresh();
	}

	void refresh()
	{
		if (sampleMapId->getSelectedId() == 0)
		{
			showStatusMessage("Select a samplemap you want to apply");
			return;
		}

		auto bf = getBackupFolder();
		auto exists = bf.isDirectory();

		if (exists)
			showStatusMessage("Press OK to restore the backup from /" + bf.getFileName());
		else
			showStatusMessage("Press OK to move the original files to /" + bf.getFileName() + " and apply the properties");
	}

	File getBackupFolder()
	{
		auto sampleFolder = getMainController()->getCurrentFileHandler().getRootFolder().getChildFile("SampleBackups");
		sampleFolder.createDirectory();
		auto t = getComboBoxComponent("samplemapId")->getText().fromLastOccurrenceOf("}", false, false);
		t << getTextEditor("backup_postfix")->getText();
		return sampleFolder.getChildFile(t);
	}

	File getSampleFolder() const
	{
		return getMainController()->getCurrentFileHandler().getSubDirectory(FileHandlerBase::Samples);
	}

	void threadFinished() override
	{
		getMainController()->getCurrentFileHandler().pool->getSampleMapPool().refreshPoolAfterUpdate();

		if (!result.wasOk())
		{
			PresetHandler::showMessageWindow("Error at applying properties", result.getErrorMessage(), PresetHandler::IconType::Error);
		}
		else
		{
			if (doBackup)
			{
				if (PresetHandler::showYesNoWindow("OK", "The backup was successfully restored. Do you want to delete the backup folder?"))
				{
					getBackupFolder().deleteRecursively();
				}
			}
			else
			{
				String m;

				m << "The samplemap was applied and saved as backup";

				if (wasMonolith)
					m << "  \n> The monolith files were also removed, so you can reencode the samplemap";

				PresetHandler::showMessageWindow("OK", m);
			}
		}
	}

	struct SampleWithPropertyData: public ReferenceCountedObject
	{
		using List = ReferenceCountedArray<SampleWithPropertyData>;

		SampleWithPropertyData() = default;

		void addFileFromValueTree(MainController* mc, const ValueTree& v)
		{
			if (v.hasProperty(SampleIds::FileName))
			{
				PoolReference sRef(mc, v[SampleIds::FileName].toString(), FileHandlerBase::SubDirectories::Samples);

				if (sRef.isAbsoluteFile())
				{
					String s;
					s << "Absolute file reference detected  \n";
					s << "> " << sRef.getFile().getFullPathName() << "\n";
					throw Result::fail(s);
				}

				sampleFiles.add(sRef.getFile());
			}

			for (auto c : v)
				addFileFromValueTree(mc, c);
		}

		bool operator==(const SampleWithPropertyData& other) const
		{
			for (auto& tf : sampleFiles)
			{
				for (auto& otf : other.sampleFiles)
				{
					if (tf == otf)
						return true;
				}
			}

			return false;
		}

		void addDelta(int delta, const Array<Identifier>& otherIds)
		{
			for (auto i : otherIds)
			{
				if (propertyData.hasProperty(i))
				{
					auto newValue = (int)propertyData[i] + delta;
					propertyData.setProperty(i, newValue, nullptr);
				}
			}
		}

		void addFactor(double factor, const Array<Identifier>& otherIds)
		{
			for (auto i : otherIds)
			{
				if (propertyData.hasProperty(i))
				{
					auto newValue = (double)propertyData[i] * factor;
					propertyData.setProperty(i, (int)(newValue), nullptr);
				}
			}
		}

		void apply(const Identifier& id)
		{
			for (auto f : sampleFiles)
			{
				apply(id, f);
			}
		}

		void apply(const Identifier& id, File& fileToUse)
		{
			if (!propertyData.hasProperty(id) || propertyData[id].toString().getIntValue() == 0)
				return;

			auto value = propertyData[id];

			double unused = 0;
			auto ob = hlac::CompressionHelpers::loadFile(fileToUse, unused);

			int numChannels = ob.getNumChannels();
			int numSamples = ob.getNumSamples();

			fileToUse.deleteFile();

			AudioSampleBuffer lut;
			bool isTable = getPropertyIds(Preset::Tables).contains(id);

			if (isTable)
			{
				SampleLookupTable t;
				t.fromBase64String(propertyData[id].toString());
				lut = AudioSampleBuffer(1, numSamples);
				t.fillExternalLookupTable(lut.getWritePointer(0), numSamples);
			}

			if (id == SampleIds::Volume)
			{
				float gainFactor = Decibels::decibelsToGain((float)value);
				ob.applyGain(gainFactor);
			}
			else if (id == SampleIds::Normalized)
			{
				auto gainFactor = (float)propertyData[SampleIds::NormalizedPeak];
				ob.applyGain(gainFactor);
				propertyData.removeProperty(SampleIds::NormalizedPeak, nullptr);
			}
			else if (id == SampleIds::SampleStart)
			{
				int offset = (int)value;
				AudioSampleBuffer nb(numChannels, numSamples - offset);

				for (int i = 0; i < numChannels; i++)
					FloatVectorOperations::copy(nb.getWritePointer(i), ob.getReadPointer(i, offset), nb.getNumSamples());

				addDelta(-offset, { SampleIds::SampleEnd, SampleIds::LoopStart, SampleIds::LoopEnd });

				std::swap(nb, ob);
			}
			else if (id == SampleIds::LoopXFade)
			{
				int xfadeSize = (int)value;
				AudioSampleBuffer loopBuffer(numChannels, xfadeSize);

				loopBuffer.clear();
			
				float fadeOutStart = (int)propertyData[SampleIds::LoopEnd] - xfadeSize;
				auto  fadeInStart = (int)propertyData[SampleIds::LoopStart] - xfadeSize;

				ob.applyGainRamp(fadeOutStart, xfadeSize, 1.0f, 0.0f);

				for (int i = 0; i < numChannels; i++)
					ob.addFromWithRamp(i, fadeOutStart, ob.getReadPointer(i, fadeInStart), xfadeSize, 0.0f, 1.0f);

			}
			else if (id == SampleIds::SampleEnd)
			{
				int length = (int)value;
				AudioSampleBuffer nb(numChannels, length);

				for (int i = 0; i < numChannels; i++)
					FloatVectorOperations::copy(nb.getWritePointer(i), ob.getReadPointer(i, 0), length);

				std::swap(nb, ob);
			}
			else if (id == SampleIds::Pitch || id == SampleIds::PitchTable)
			{
				int newNumSamples = 0;

				auto getPitchFactor = [&](int index)
				{
					if (isTable)
						return (double)ModulatorSamplerSound::EnvelopeTable::getPitchValue(lut.getSample(0, index));
					else
						return scriptnode::conversion_logic::st2pitch().getValue((double)propertyData[id] / 100.0);
				};

				if (isTable)
				{
					double samplesToCalculate = 0.0;

					for (int i = 0; i < numSamples; i++)
						samplesToCalculate += 1.0 / getPitchFactor(i);

					newNumSamples = (int)samplesToCalculate;
				}
				else
					newNumSamples = (int)((double)numSamples / getPitchFactor(0));

				AudioSampleBuffer nb(numChannels, newNumSamples);
				
				ValueTree tableIndexes;

				Array<Identifier> sampleRangeIds = { SampleIds::SampleStart, SampleIds::SampleEnd,
						SampleIds::LoopStart, SampleIds::LoopEnd,
						SampleIds::LoopXFade, SampleIds::SampleStartMod };

				if (isTable)
				{
					tableIndexes = ValueTree("Ranges");

					for (auto id : sampleRangeIds)
					{
						if (propertyData.hasProperty(id))
							tableIndexes.setProperty(id, propertyData[id], nullptr);
					}
				}
				
				double uptime = 0.0;

				for (int i = 0; i < newNumSamples; i++)
				{
					auto i0 = jmin(numSamples-1, (int)uptime);
					auto i1 = jmin(numSamples-1, i0 + 1);
					auto alpha = uptime - (double)i0;
					
					for (int c = 0; c < numChannels; c++)
					{
						auto v0 = ob.getSample(c, i0);
						auto v1 = ob.getSample(c, i1);
						auto v = Interpolator::interpolateLinear(v0, v1, (float)alpha);
						nb.setSample(c, i, v);
					}

					auto uptimeDelta = getPitchFactor(0);

					if (isTable)
					{
						for (int i = 0; i < tableIndexes.getNumProperties(); i++)
						{
							auto id = tableIndexes.getPropertyName(i);

							if ((int)tableIndexes[id] == i)
								tableIndexes.setProperty(id, (int)uptime, nullptr);
						}

						auto pf0 = getPitchFactor(i0);
						auto pf1 = getPitchFactor(i1);
						uptimeDelta = Interpolator::interpolateLinear(pf0, pf1, alpha);
					}

					uptime += uptimeDelta;
				}

				if (isTable)
				{
					for (int i = 0; i < tableIndexes.getNumProperties(); i++)
					{
						auto id = tableIndexes.getPropertyName(i);
						propertyData.setProperty(id, tableIndexes[id], nullptr);
					}
				}
				else
				{
					// Calculate the static offset
					addFactor(1.0 / getPitchFactor(0), sampleRangeIds);
				}

				std::swap(ob, nb);
			}
			else if(id == SampleIds::GainTable)
			{
				for (int c = 0; c < numChannels; c++)
				{
					for (int i = 0; i < numSamples; i++)
					{
						auto gainFactor = ModulatorSamplerSound::EnvelopeTable::getGainValue(lut.getSample(0, i));
						auto value = ob.getSample(c, i);
						ob.setSample(c, i, value * gainFactor);
					}
				}
			}
			else if (id == SampleIds::LowPassTable)
			{
				CascadedEnvelopeLowPass lp(true);

				PrepareSpecs ps;
				ps.blockSize = 16;
				ps.sampleRate = 44100.0;
				ps.numChannels = numChannels;

				lp.prepare(ps);

				snex::PolyHandler::ScopedVoiceSetter svs(lp.polyManager, 0);

				for (int i = 0; i < numSamples; i+= ps.blockSize)
				{
					int numToDo = jmin(ps.blockSize, numSamples - i);
					auto v = lut.getSample(0, i);
					auto freq = ModulatorSamplerSound::EnvelopeTable::getFreqValue(v);
					lp.process(freq, ob, i, numToDo);
				}
			}

			propertyData.removeProperty(id, nullptr);
			hlac::CompressionHelpers::dump(ob, fileToUse.getFullPathName());
		}

		ValueTree propertyData;
		Array<File> sampleFiles;

		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SampleWithPropertyData);
	};

	File getSampleMapFile(bool fromBackup)
	{
		if (fromBackup)
			return getBackupFolder().getChildFile(sampleMapId->getText()).withFileExtension("xml");
		else
		{
			PoolReference ref(getMainController(), sampleMapId->getText(), FileHandlerBase::SampleMaps);
			return ref.getFile();
		}
	}

	Array<Identifier> getPropertiesToProcess()
	{
		Array<Identifier> ids;
		for (auto i : propertySelector->items)
		{
			if (!i->active)
				continue;

			ids.add(Identifier(i->id));
		}

		return ids;
	}

	SampleWithPropertyData::List saveToBackup()
	{
		SampleWithPropertyData::List fileList;

		showStatusMessage("Saving backup");

		auto smf = getSampleMapFile(false);

		auto v = ValueTree::fromXml(smf.loadFileAsString());

		if (!v.isValid())
			throw Result::fail("Can't load samplemap");

		wasMonolith = (int)v[Identifier("SaveMode")] != 0;

		if (wasMonolith)
		{
			getMainController()->getSampleManager().getModulatorSamplerSoundPool2()->clearUnreferencedMonoliths();

			auto monolist = getSampleFolder().findChildFiles(File::findFiles, false);

			for (auto m : monolist)
			{
				if (m.getFileExtension().startsWith(".ch") && m.getFileNameWithoutExtension() == sampleMapId->getText())
				{
					auto ok = m.deleteFile();

					if (!ok)
						throw Result::fail("Can't delete monolith");
				}
			}
		}

		auto bf = getBackupFolder();
		auto r = bf.createDirectory();

		auto ok = smf.copyFileTo(getSampleMapFile(true));
		
		if (!ok)
			throw Result::fail("Can't copy samplemap file");

		if (!r.wasOk())
			throw r;

		for (auto c : v)
		{
			ReferenceCountedObjectPtr<SampleWithPropertyData> newData = new SampleWithPropertyData();

			newData->propertyData = c;
			newData->addFileFromValueTree(getMainController(), c);
			
			fileList.add(newData);
		}

		showStatusMessage("Copying sample files to backup location");

		double numTodo = (double)fileList.size();
		double numDone = 0.0;

		for (auto sf : fileList)
		{
			setProgress(numDone / numTodo);

			for (auto sourceFile : sf->sampleFiles)
			{
				auto targetFilePath = sourceFile.getRelativePathFrom(getSampleFolder());
				auto targetFile = bf.getChildFile(targetFilePath);

				if (!targetFile.getParentDirectory().isDirectory())
				{
					r = targetFile.getParentDirectory().createDirectory();

					if (!r.wasOk())
						throw r;
				}

				auto ok = sourceFile.copyFileTo(targetFile);

				if (!ok)
					throw Result::fail("Can't copy sample file");
			}
		}

		return fileList;
	}

	void restoreFromBackup()
	{
		auto smf = getSampleMapFile(true);

		if (!smf.existsAsFile())
		{
			throw Result::fail("Can't find samplemap backup file");
		}

		auto ok = smf.copyFileTo(getSampleMapFile(false));

		if (!ok)
			throw Result::fail("Can't copy samplemap backup");
		
		auto fileList = getBackupFolder().findChildFiles(File::findFiles, true, "*");

		for (auto f : fileList)
		{
			if (f == smf)
				continue;

			if (f.isHidden())
				continue;

			auto targetPath = f.getRelativePathFrom(getBackupFolder());
			auto targetFile = getSampleFolder().getChildFile(targetPath);

			if (targetFile.existsAsFile())
			{
				auto ok = targetFile.deleteFile();

				if (!ok)
					throw Result::fail("Can't delete file  \n> " + targetFile.getFullPathName());
			}

			auto ok = f.copyFileTo(targetFile);

			if (!ok)
				throw Result::fail("Can't copy file  \n>" + f.getFullPathName());
		}
	}

	static void removeProperties(ValueTree v, const Array<Identifier>& properties)
	{
		for (auto p : properties)
			v.removeProperty(p, nullptr);

		for (auto c : v)
			removeProperties(c, properties);
	}

	void applyChanges(SampleWithPropertyData::List& sampleList)
	{
		auto propertiesToProcess = getPropertiesToProcess();

		double numTodo = sampleList.size();
		double numDone = 0.0;

		for (auto s : sampleList)
		{
			setProgress(numDone / numTodo);
			numDone += 1.0;

			for (const auto& p : propertiesToProcess)
			{
				s->apply(p);
			}
		}

		auto v = ValueTree::fromXml(getSampleMapFile(true).loadFileAsString());

		
		v.removeAllChildren(nullptr);

		for (auto s : sampleList)
		{
			v.addChild(s->propertyData.createCopy(), -1, nullptr);
		}

		if (wasMonolith)
		{
			// set it to use the files
			v.setProperty("SaveMode", 0, nullptr);

			// remove the monolith info
			removeProperties(v, { Identifier("MonolithLength"), Identifier("MonolithOffset") });
		}

		auto xml = v.createXml();
		
		getSampleMapFile(false).replaceWithText(xml->createDocument(""));
	}

	void run() override
	{
		auto bf = getBackupFolder();

		try
		{
			doBackup = bf.isDirectory();

			if (doBackup)
			{
				restoreFromBackup();
			}
			else
			{
				auto propertiesToProcess = getPropertiesToProcess();

				if (propertiesToProcess.isEmpty())
				{
					throw Result::fail("No properties selected");
				}

				auto sampleList = saveToBackup();
				applyChanges(sampleList);
			}
		}
		catch (Result& r)
		{
			result = r;
		}
	}

	Result result;

	bool doBackup = false;
	bool wasMonolith = false;

	ComboBox* sampleMapId;
	TextEditor* suffix;
	ScopedPointer<PropertySelector> propertySelector;
};

class SVGToPathDataConverter : public Component,
							   public Value::Listener,
							   public QuasiModalComponent,
							   public PathFactory,
							   public juce::FileDragAndDropTarget
{
public:

	enum class OutputFormat
	{
		Base64,
		CppString,
		HiseScriptNumbers,
        Base64SVG,
		numOutputFormats
	};

	SVGToPathDataConverter(BackendRootWindow* bpe):
		QuasiModalComponent(),
		loadClipboard("Load from clipboard"),
		copyClipboard("Copy to clipboard"),
		resizer(this, nullptr),
		closeButton("close", nullptr, *this)
	{
		outputFormatSelector.addItemList({ "Base64 Path", "C++ Path String", "HiseScript Path number array", "Base64 SVG" }, 1);
		
		addAndMakeVisible(outputFormatSelector);
		addAndMakeVisible(inputEditor);
		addAndMakeVisible(outputEditor);
		addAndMakeVisible(variableName);
		addAndMakeVisible(loadClipboard);
		addAndMakeVisible(copyClipboard);
		addAndMakeVisible(resizer);
		addAndMakeVisible(closeButton);

		GlobalHiseLookAndFeel::setTextEditorColours(inputEditor);
		GlobalHiseLookAndFeel::setTextEditorColours(outputEditor);
		inputEditor.setFont(GLOBAL_MONOSPACE_FONT());
		outputEditor.setFont(GLOBAL_MONOSPACE_FONT());
		GlobalHiseLookAndFeel::setTextEditorColours(variableName);
		
		inputEditor.setFont(GLOBAL_MONOSPACE_FONT());
		variableName.setFont(GLOBAL_MONOSPACE_FONT());

		inputEditor.setMultiLine(true);
		outputEditor.setMultiLine(true);

		inputEditor.getTextValue().referTo(inputDoc);
		outputEditor.getTextValue().referTo(outputDoc);
		variableName.getTextValue().referTo(variableDoc);
		variableDoc.addListener(this);

		variableDoc.setValue("pathData");

		outputFormatSelector.setSelectedItemIndex(0);

		copyClipboard.setLookAndFeel(&alaf);
		loadClipboard.setLookAndFeel(&alaf);
		outputFormatSelector.setLookAndFeel(&alaf);

		outputFormatSelector.onChange = [&]()
		{
			currentOutputFormat = (OutputFormat)outputFormatSelector.getSelectedItemIndex();
			update();
		};

		loadClipboard.onClick = [&]()
		{
			inputDoc.setValue(SystemClipboard::getTextFromClipboard());
		};

		copyClipboard.onClick = [&]()
		{
			SystemClipboard::copyTextToClipboard(outputDoc.getValue().toString());
		};

		GlobalHiseLookAndFeel::setDefaultColours(outputFormatSelector);

        inputDoc.setValue("Paste the SVG data here, drop a SVG file or use the Load from Clipboard button.\nThen select the output format xand variable name above, and click Copy to Clipboard to paste the path data.\nYou can also paste an array that you've previously exported to convert it to Base64");
        
		inputDoc.addListener(this);

		closeButton.onClick = [this]()
		{
			this->destroy();
		};

		setSize(800, 600);
	}

	~SVGToPathDataConverter()
	{
		inputDoc.removeListener(this);
		variableDoc.removeListener(this);
	}

	void valueChanged(Value& v) override
	{
		update();
	}

	void mouseDown(const MouseEvent& e) override
	{
		dragger.startDraggingComponent(this, e);
	}

	void mouseDrag(const MouseEvent& e) override
	{
		dragger.dragComponent(this, e, nullptr);
	}

	void mouseUp(const MouseEvent& e) override
	{
		
	}

	static bool isHiseScriptArray(const String& input)
	{
		return input.startsWith("const var") || input.startsWith("[");
	}

	static String parse(const String& input, OutputFormat format)
	{
		String rt = input.trim();

		if (isHiseScriptArray(rt) || format == OutputFormat::Base64SVG)
		{
			return rt;
		}

		if (auto xml = XmlDocument::parse(input))
		{
			auto v = ValueTree::fromXml(*xml);

			valuetree::Helpers::forEach(v, [&](ValueTree& c)
			{
				if (c.hasType("path"))
				{
					rt = c["d"].toString();
					return true;
				}

				return false;
			});
		}

		return rt;
	}

	Path createPath(const String& url) const override
	{
		Path p;
		p.loadPathFromData(HiBinaryData::ProcessorEditorHeaderIcons::closeIcon, SIZE_OF_PATH(HiBinaryData::ProcessorEditorHeaderIcons::closeIcon));
		return p;
	}

	Path pathFromPoints(String pointsText)
	{
		auto points = StringArray::fromTokens(pointsText, " ,", "");
		points.removeEmptyStrings();

		jassert(points.size() % 2 == 0);

		Path p;

		for (int i = 0; i < points.size() / 2; i++)
		{
			auto x = points[i * 2].getFloatValue();
			auto y = points[i * 2 + 1].getFloatValue();

			if (i == 0)
				p.startNewSubPath({ x, y });
			else
				p.lineTo({ x, y });
		}

		p.closeSubPath();

		return p;
	}

	bool isInterestedInFileDrag(const StringArray& files) override
	{
		return File(files[0]).getFileExtension() == ".svg";
	}

	void filesDropped(const StringArray& files, int x, int y) override
	{
		File f(files[0]);
		auto filename = f.getFileNameWithoutExtension();
		variableDoc.setValue(filename);
		inputDoc.setValue(f.loadFileAsString());
	}

	void writeDataAsCppLiteral(const MemoryBlock& mb, OutputStream& out,
		bool breakAtNewLines, bool allowStringBreaks, String bracketSet = "{}")
	{
		const int maxCharsOnLine = 250;

		auto data = (const unsigned char*)mb.getData();
		int charsOnLine = 0;

		bool canUseStringLiteral = mb.getSize() < 32768; // MS compilers can't handle big string literals..

		if (canUseStringLiteral)
		{
			unsigned int numEscaped = 0;

			for (size_t i = 0; i < mb.getSize(); ++i)
			{
				auto num = (unsigned int)data[i];

				if (!((num >= 32 && num < 127) || num == '\t' || num == '\r' || num == '\n'))
				{
					if (++numEscaped > mb.getSize() / 4)
					{
						canUseStringLiteral = false;
						break;
					}
				}
			}
		}

		if (!canUseStringLiteral)
		{
			out << bracketSet.substring(0, 1) << " ";

			for (size_t i = 0; i < mb.getSize(); ++i)
			{
				auto num = (int)(unsigned int)data[i];
				out << num << ',';

				charsOnLine += 2;

				if (num >= 10)
				{
					++charsOnLine;

					if (num >= 100)
						++charsOnLine;
				}

				if (charsOnLine >= maxCharsOnLine)
				{
					charsOnLine = 0;
					out << newLine;
				}
			}

			out << "0,0 " << bracketSet.substring(1) << ";";
		}
		
	}

	void update()
	{
        currentSVG = nullptr;
        path = Path();
        
		auto inputText = parse(inputDoc.toString(), currentOutputFormat);

		String result = "No path generated.. Not a valid SVG path string?";

		if (isHiseScriptArray(inputText))
		{
			auto ar = JSON::parse(inputText.fromFirstOccurrenceOf("[", true, true));

			if (ar.isArray())
			{
				MemoryOutputStream mos;

				for (auto v : *ar.getArray())
				{
					auto byte = (uint8)(int)v;
					mos.write(&byte, 1);
				}
				
				mos.flush();

				path.clear();
				path.loadPathFromData(mos.getData(), mos.getDataSize());

				auto b64 = mos.getMemoryBlock().toBase64Encoding();

				result = {};

				if (!inputText.startsWith("["))
					result << inputText.upToFirstOccurrenceOf("[", false, false);

				result << b64.quoted();

				if (inputText.endsWith(";"))
					result << ";";
			}
		}
		else
		{
			auto text = inputText.trim().unquoted().trim();

            if(currentOutputFormat == OutputFormat::Base64SVG)
            {
                if(auto xml = XmlDocument::parse(text))
                {
                    currentSVG = Drawable::createFromSVG(*xml);
                    
                    currentSVG->setTransformToFit(pathArea, RectanglePlacement::centred);
                }
            }
            else
            {
                path = Drawable::parseSVGPath(text);

                if (path.isEmpty())
                    path = pathFromPoints(text);
                
                if(!path.isEmpty())
                    PathFactory::scalePath(path, pathArea);
            }

            auto filename = snex::cppgen::StringHelpers::makeValidCppName(variableDoc.toString());

			if (!path.isEmpty() || currentSVG != nullptr)
			{
				MemoryOutputStream data;
                
                MemoryBlock mb;
                
                if(currentSVG != nullptr)
                {
                    zstd::ZDefaultCompressor comp;
                    
                    comp.compress(text, mb);
                }
				else
				{
					path.writePathToStream(data);
					mb = data.getMemoryBlock();
				}
                
				MemoryOutputStream out;

				if (currentOutputFormat == OutputFormat::CppString)
				{
					out << "static const unsigned char " << filename << "[] = ";

					writeDataAsCppLiteral(mb, out, false, true, "{}");

					out << newLine
						<< newLine
						<< "Path path;" << newLine
						<< "path.loadPathFromData (" << filename << ", sizeof (" << filename << "));" << newLine;

				}
				else if (currentOutputFormat == OutputFormat::Base64 || currentOutputFormat == OutputFormat::Base64SVG)
				{
					out << "const var " << filename << " = ";
					out << "\"" << mb.toBase64Encoding() << "\"";
				}
				else if (currentOutputFormat == OutputFormat::HiseScriptNumbers)
				{
					out << "const var " << filename << " = ";
					writeDataAsCppLiteral(mb, out, false, true, "[]");

					out << ";";
				}

				result = out.toString();
			}
		}
		
		outputDoc.setValue(result);

		

		repaint();
	}

	void paint(Graphics& g)
	{
		ColourGradient grad(alaf.dark.withMultipliedBrightness(1.4f), 0.0f, 0.0f,
			alaf.dark, 0.0f, (float)getHeight(), false);

		g.setGradientFill(grad);
		g.fillAll();
		g.setColour(Colours::white.withAlpha(0.1f));
		g.fillRect(getLocalBounds().removeFromTop(37).toFloat());
		g.setColour(alaf.bright);

		g.drawRect(getLocalBounds().toFloat());

		g.setFont(GLOBAL_BOLD_FONT().withHeight(17.0f));
		g.drawText("SVG to Path converter", titleArea, Justification::centred);

        if(currentSVG != nullptr)
        {
            currentSVG->draw(g, 1.0f);
        }
        else if(path.isEmpty())
        {
            g.setFont(GLOBAL_BOLD_FONT());
            g.drawText("No valid path", pathArea, Justification::centred);
        }
        else
        {
            g.fillPath(path);
            g.strokePath(path, PathStrokeType(1.0f));
        }
	}

	void resized() override
	{
		auto b = getLocalBounds();
		
		titleArea = b.removeFromTop(37).toFloat();

		b = b.reduced(10);

		auto top = b.removeFromTop(32);


		
		outputFormatSelector.setBounds(top.removeFromLeft(300));

		top.removeFromLeft(5);

		variableName.setBounds(top.removeFromLeft(200));

		auto bottom = b.removeFromBottom(32);

		b.removeFromBottom(5);

		bottom.removeFromRight(15);

		auto w = getWidth() / 3;

		inputEditor.setBounds(b.removeFromLeft(w-5));
		b.removeFromLeft(5);
		outputEditor.setBounds(b.removeFromLeft(w-5));
		b.removeFromLeft(5);
		pathArea = b.toFloat();

		loadClipboard.setBounds(bottom.removeFromLeft(150));
		bottom.removeFromLeft(10);
		copyClipboard.setBounds(bottom.removeFromLeft(150));

		resizer.setBounds(getLocalBounds().removeFromRight(15).removeFromBottom(15));
		closeButton.setBounds(getLocalBounds().removeFromRight(titleArea.getHeight()).removeFromTop(titleArea.getHeight()).reduced(6));

        if(!path.isEmpty())
            scalePath(path, pathArea);
        else if(currentSVG != nullptr)
            currentSVG->setTransformToFit(pathArea, RectanglePlacement::centred);
            
		repaint();
	}

    std::unique_ptr<Drawable> currentSVG;
	Path path;
	Rectangle<float> pathArea;
	Rectangle<float> titleArea;

	Value inputDoc, outputDoc, variableDoc;

	TextEditor inputEditor, outputEditor;
	TextEditor variableName;

	ComboBox outputFormatSelector;

	OutputFormat currentOutputFormat = OutputFormat::Base64;
	TextButton loadClipboard, copyClipboard;

	ResizableCornerComponent resizer;
	HiseShapeButton closeButton;
	AlertWindowLookAndFeel alaf;
	juce::ComponentDragger dragger;
};

class ProjectDownloader : public DialogWindowWithBackgroundThread,
	public TextEditor::Listener
{
public:

	enum class ErrorCodes
	{
		OK = 0,
		InvalidURL,
		URLNotFound,
		DirectoryAlreadyExists,
		FileNotAnArchive,
		AbortedByUser,
		numErrorCodes
	};

	ProjectDownloader(BackendProcessorEditor *bpe_) :
		DialogWindowWithBackgroundThread("Download new Project"),
		bpe(bpe_),
		result(ErrorCodes::OK)
	{
		addTextEditor("url", "http://www.", "URL");

#if HISE_IOS

		addTextEditor("projectName", "Project", "Project Name");

#else

		targetFile = new FilenameComponent("Target folder", File(), true, true, true, "", "", "Choose target folder");
		targetFile->setSize(300, 24);
		addCustomComponent(targetFile);

#endif

		addBasicComponents(true);
		addButton("Paste URL from Clipboard", 2);
	};

	void resultButtonClicked(const String &name)
	{
		if (name == "Paste URL from Clipboard")
		{
			getTextEditor("url")->setText(SystemClipboard::getTextFromClipboard());
		}
	}

	void run() override
	{
#if HISE_IOS
		targetDirectory = File::getSpecialLocation(File::userDocumentsDirectory).getChildFile(getTextEditor("projectName")->getText());

		if (targetDirectory.isDirectory())
		{
			result = ErrorCodes::DirectoryAlreadyExists;
			return;
		}

		targetDirectory.createDirectory();

#else
		targetDirectory = targetFile->getCurrentFile();

		if (targetDirectory.isDirectory() && targetDirectory.getNumberOfChildFiles(File::findFilesAndDirectories) != 0)
		{
			result = ErrorCodes::DirectoryAlreadyExists;
			return;
		}

#endif

		const String enteredURL = getTextEditor("url")->getText();

		const String directURL = replaceHosterLinksWithDirectDownloadURL(enteredURL);

		URL downloadLocation(directURL);

		if (!downloadLocation.isWellFormed())
		{
			result = ErrorCodes::InvalidURL;
			targetDirectory.deleteRecursively();
			return;
		}

		showStatusMessage("Downloading the project");

		auto stream = downloadLocation.createInputStream(false, &downloadProgress, this, String(), 0, nullptr, &httpStatusCode, 20);

		if (stream == nullptr || stream->getTotalLength() <= 0)
		{
			result = ErrorCodes::URLNotFound;
			targetDirectory.deleteRecursively();
			return;
		}

		File tempFile(File::getSpecialLocation(File::tempDirectory).getChildFile("projectDownload.tmp"));

		tempFile.deleteFile();
		tempFile.create();

		auto fos = tempFile.createOutputStream();

		MemoryBlock mb;
		mb.setSize(8192);

		const int64 numBytesTotal = stream->getNumBytesRemaining();
		int64 numBytesRead = 0;

		while (stream->getNumBytesRemaining() > 0)
		{
			const int64 chunkSize = (int64)jmin<int>((int)stream->getNumBytesRemaining(), 8192);

			downloadProgress(this, (int)numBytesRead, (int)numBytesTotal);

			if (threadShouldExit())
			{
				result = ErrorCodes::AbortedByUser;
				fos->flush();
				fos = nullptr;

				tempFile.deleteFile();
				targetDirectory.deleteRecursively();
				return;
			}

			stream->read(mb.getData(), (int)chunkSize);

			numBytesRead += chunkSize;

			fos->write(mb.getData(), (size_t)chunkSize);
		}

		fos->flush();

		showStatusMessage("Extracting...");

		setProgress(-1.0);

		FileInputStream fis(tempFile);

		ZipFile input(&fis, false);

		if (input.getNumEntries() == 0)
		{
			result = ErrorCodes::FileNotAnArchive;
			tempFile.deleteFile();
			targetDirectory.deleteRecursively();
			return;
		}

		const int numFiles = input.getNumEntries();

		for (int i = 0; i < numFiles; i++)
		{
			if (threadShouldExit())
			{
				tempFile.deleteFile();
				targetDirectory.deleteRecursively();
				result = ErrorCodes::AbortedByUser;

				break;
			}

			input.uncompressEntry(i, targetDirectory, true);

			setProgress((double)i / (double)numFiles);
		}

		tempFile.deleteFile();

	}


	static bool downloadProgress(void* context, int bytesSent, int totalBytes)
	{
		const double downloadedMB = (double)bytesSent / 1024.0 / 1024.0;
		const double totalMB = (double)totalBytes / 1024.0 / 1024.0;
		const double percent = (totalMB > 0.0) ? (downloadedMB / totalMB) : 0.0;

		static_cast<ProjectDownloader*>(context)->showStatusMessage("Downloaded: " + String(downloadedMB, 1) + " MB / " + String(totalMB, 2) + " MB");

		static_cast<ProjectDownloader*>(context)->setProgress(percent);

		return !static_cast<ProjectDownloader*>(context)->threadShouldExit();
	}

	void threadFinished() override
	{
		switch (result)
		{
		case ProjectDownloader::ErrorCodes::OK:
			if (PresetHandler::showYesNoWindow("Switch projects", "Do you want to switch to the downloaded project?", PresetHandler::IconType::Question))
			{
				GET_PROJECT_HANDLER(bpe->getMainSynthChain()).setWorkingProject(targetDirectory);
			}
			break;
		case ProjectDownloader::ErrorCodes::InvalidURL:
			PresetHandler::showMessageWindow("Wrong URL", "The entered URL is not valid", PresetHandler::IconType::Error);
			break;
		case ProjectDownloader::ErrorCodes::URLNotFound:
			PresetHandler::showMessageWindow("Error downloading", "The URL could not be opened. HTTP status code: " + String(httpStatusCode), PresetHandler::IconType::Error);
			break;
		case ProjectDownloader::ErrorCodes::DirectoryAlreadyExists:
			PresetHandler::showMessageWindow("Project already exists.", "You'll need to delete the existing project before downloading.", PresetHandler::IconType::Error);
			break;
		case ProjectDownloader::ErrorCodes::FileNotAnArchive:
			PresetHandler::showMessageWindow("Archive corrupt", "The file could not be extracted because it is either corrupt or not an archive.", PresetHandler::IconType::Error);
		case ProjectDownloader::ErrorCodes::AbortedByUser:
			PresetHandler::showMessageWindow("Download cancelled", "The project was not downloaded because the progress was aborted.", PresetHandler::IconType::Error);
		case ProjectDownloader::ErrorCodes::numErrorCodes:
			break;
		default:
			break;
		}


	}

private:

	/** A small helper function that replaces links to cloud content with their direct download URL. */
	static String replaceHosterLinksWithDirectDownloadURL(const String url)
	{
		const bool dropBox = url.containsIgnoreCase("dropbox");
		const bool googleDrive = url.containsIgnoreCase("drive.google.com");

		if (dropBox)
		{
			return url.replace("dl=0", "dl=1");;
		}
		else if (googleDrive)
		{
			const String downloadID = url.fromFirstOccurrenceOf("https://drive.google.com/file/d/", false, true).upToFirstOccurrenceOf("/", false, false);
			const String directLink = "https://drive.google.com/uc?export=download&id=" + downloadID;

			return directLink;
		}
		else return url;
	}

	BackendProcessorEditor *bpe;

	ScopedPointer<FilenameComponent> targetFile;

	File targetDirectory;

	ErrorCodes result;
	int httpStatusCode;
};


class ProjectImporter : public DialogWindowWithBackgroundThread,
						 public ControlledObject,
						 public URL::DownloadTaskListener,
						 public hlac::HlacArchiver::Listener
{
public:

	enum class SourceType
	{
		New,
		Import,
		Template
	};

	struct Header : public Component,
					public ButtonListener,
					public PathFactory,
					public TextEditor::Listener
	{
		Header(ProjectImporter& parent_):
			parent(parent_),
			newButton("new", this, *this),
			importButton("import", this, *this),
			templateButton("template", this, *this),
			browse("BROWSE")
		{
			folder.setFont(GLOBAL_BOLD_FONT());
			addAndMakeVisible(newButton);
			addAndMakeVisible(importButton);
			addAndMakeVisible(templateButton);

			addAndMakeVisible(browse);
			addAndMakeVisible(folder);

			folder.setMultiLine(false);
			folder.setTextToShowWhenEmpty("Please select an empty folder", Colours::grey);

			browse.addListener(this);

			setSize(600, 300);
		}

		bool checkEnablement()
		{
			auto isFolder = parent.newProjectFolder.isDirectory();
			
			auto numFiles = parent.newProjectFolder.getNumberOfChildFiles(File::findFilesAndDirectories);

			auto isEmpty = numFiles == 0;

			// allow the .git folder to exist when creating a project...
			if (!isEmpty && numFiles == 1 && parent.newProjectFolder.getChildFile(".git").isDirectory())
				isEmpty = true;

			auto ok = isFolder && isEmpty;

			if (!isFolder)
				parent.showStatusMessage("Please select an empty folder for the project");
			else if (!isEmpty)
				parent.showStatusMessage("The project folder must be empty. Please select an empty folder");
			else if (ok)
				parent.showStatusMessage("Please choose one of the options for creating the Project");

			importButton.setEnabled(ok);
			newButton.setEnabled(ok);
			templateButton.setEnabled(ok);

			return ok;
		}

		void textEditorTextChanged(TextEditor& te) override
		{
			auto content = te.getText();

			if (File::isAbsolutePath(content))
			{
				File r(content);

				if (r.isDirectory() && r.getNumberOfChildFiles(File::findFilesAndDirectories) == 0)
				{
					parent.newProjectFolder = r;
				}

				checkEnablement();
			}
		}

		void buttonClicked(Button* b) override
		{
			if (b == &browse || !parent.newProjectFolder.isDirectory())
			{
				FileChooser fc("Select project folder");
				
				if (fc.browseForDirectory())
				{
					parent.newProjectFolder = fc.getResult();
					folder.setText(fc.getResult().getFullPathName(), dontSendNotification);

					if (!checkEnablement())
						return;

				}
				else
					return;
			}

			if (b == &newButton)
			{
				parent.sourceType = SourceType::New;
				parent.runThread();
			}
			if (b == &importButton)
			{
				parent.sourceType = SourceType::Import;

				FileChooser fc("Select HXI file to import", File(), "*.hxi;*.lwz");

				if (fc.browseForFileToOpen())
				{
					importFile = fc.getResult();
				}
				else
				{
					return;
				}

				parent.runThread();
			}
			if (b == &templateButton)
			{
				parent.sourceType = SourceType::Template;
				parent.runThread();
			}
		}

		

		void paint(Graphics& g) override
		{
			g.setFont(GLOBAL_BOLD_FONT());
			g.setColour(Colours::white.withAlpha(0.9f));
			g.drawText("Project Folder:", projectArea, Justification::centred);

			g.drawText("Empty Project", buttonTextAreas[0].toFloat(), Justification::centredTop);
			g.drawText("Import from HXI", buttonTextAreas[1].toFloat(), Justification::centredTop);
			g.drawText("RHAPSODY Template", buttonTextAreas[2].toFloat(), Justification::centredTop);

			g.setFont(GLOBAL_FONT());
			g.setColour(Colours::white.withAlpha(0.7f));

			g.drawMultiLineText("Create a project folder\nfor a new HISE Project", buttonTextAreas[0].getX(), buttonTextAreas[0].getY() + 30, buttonTextAreas[0].getWidth(), Justification::centred);
			g.drawMultiLineText("Create a HISE project from\nan HXI Expansion file", buttonTextAreas[1].getX(), buttonTextAreas[0].getY() + 30, buttonTextAreas[0].getWidth(), Justification::centred);
			g.drawMultiLineText("Create a Rhapsody expansion\nproject from the online template", buttonTextAreas[2].getX(), buttonTextAreas[0].getY() + 30, buttonTextAreas[0].getWidth(), Justification::centred);
		}

		void resized() override
		{
			auto b = getLocalBounds();

			auto top = b.removeFromTop(24 + 30 + 30).reduced(30);

			projectArea = top.removeFromLeft(90).toFloat();
			browse.setBounds(top.removeFromRight(90));
			folder.setBounds(top);

			b.translate(0, -30);

			auto bw = b.getWidth() / 3;

			buttonTextAreas.add(b.removeFromLeft(bw));
			buttonTextAreas.add(b.removeFromLeft(bw));
			buttonTextAreas.add(b.removeFromLeft(bw));

			newButton.setBounds(buttonTextAreas.getReference(0).removeFromTop(bw).reduced(55));
			importButton.setBounds(buttonTextAreas.getReference(1).removeFromTop(bw).reduced(55));
			templateButton.setBounds(buttonTextAreas.getReference(2).removeFromTop(bw).reduced(55));

			setInterceptsMouseClicks(true, true);

			for (auto& x : buttonTextAreas)
				x.translate(-20, -40);
		}

		File importFile;

		Array<Rectangle<int>> buttonTextAreas;
		Rectangle<float> projectArea;

		Path createPath(const String& id) const override
		{
			Path p;
			
			if (id == "new")
			{
				static const unsigned char pathData[] = { 110,109,138,180,7,68,176,124,205,67,108,222,173,7,68,108,190,205,67,108,140,171,7,68,90,221,205,67,98,162,141,7,68,164,164,207,67,6,35,8,68,112,6,209,67,5,14,9,68,112,6,209,67,108,85,135,40,68,112,6,209,67,98,57,133,41,68,112,6,209,67,119,135,42,68,172,
105,207,67,167,199,42,68,46,110,205,67,108,154,92,48,68,48,70,161,67,98,201,156,48,68,178,74,159,67,192,2,48,68,240,173,157,67,220,4,47,68,240,173,157,67,108,152,139,15,68,240,173,157,67,98,168,141,14,68,240,173,157,67,106,139,13,68,178,74,159,67,58,
75,13,68,48,70,161,67,108,71,182,7,68,46,110,205,67,98,178,181,7,68,6,115,205,67,30,181,7,68,216,119,205,67,138,180,7,68,176,124,205,67,99,109,0,128,7,68,78,212,179,67,108,116,13,10,68,168,162,159,67,98,47,169,10,68,38,211,154,67,152,35,13,68,172,254,
150,67,152,139,15,68,172,254,150,67,108,93,83,42,68,172,254,150,67,108,93,83,42,68,46,253,142,67,98,93,83,42,68,66,76,140,67,182,59,41,68,244,28,138,67,62,227,39,68,244,28,138,67,108,18,240,9,68,244,28,138,67,98,154,151,8,68,244,28,138,67,0,128,7,68,
66,76,140,67,0,128,7,68,46,253,142,67,108,0,128,7,68,78,212,179,67,99,109,43,185,57,68,36,243,85,67,98,15,57,64,68,36,243,85,67,0,128,69,68,180,14,107,67,0,128,69,68,60,135,130,67,98,0,128,69,68,32,135,143,67,15,57,64,68,184,20,154,67,43,185,57,68,184,
20,154,67,98,58,57,51,68,184,20,154,67,72,242,45,68,32,135,143,67,72,242,45,68,60,135,130,67,98,72,242,45,68,180,14,107,67,58,57,51,68,36,243,85,67,43,185,57,68,36,243,85,67,99,109,42,189,59,68,76,254,124,67,108,42,189,59,68,224,204,97,67,108,31,181,
55,68,224,204,97,67,108,31,181,55,68,76,254,124,67,108,195,232,48,68,76,254,124,67,108,195,232,48,68,84,143,134,67,108,31,181,55,68,84,143,134,67,108,31,181,55,68,10,40,148,67,108,42,189,59,68,10,40,148,67,108,42,189,59,68,84,143,134,67,108,133,137,66,
68,84,143,134,67,108,133,137,66,68,76,254,124,67,108,42,189,59,68,76,254,124,67,99,101,0,0 };

				p.loadPathFromData(pathData, sizeof(pathData));
			}
			if (id == "import")
			{
				static const unsigned char pathData[] = { 110,109,147,180,7,68,228,180,205,67,108,210,173,7,68,126,246,205,67,108,105,171,7,68,92,21,206,67,98,253,141,7,68,184,219,207,67,8,35,8,68,206,60,209,67,115,13,9,68,206,60,209,67,108,30,118,40,68,206,60,209,67,98,212,115,41,68,206,60,209,67,103,117,42,
68,224,160,207,67,142,181,42,68,106,166,205,67,108,93,71,48,68,96,149,161,67,98,132,135,48,68,234,154,159,67,32,238,47,68,252,254,157,67,106,240,46,68,252,254,157,67,108,191,135,15,68,252,254,157,67,98,9,138,14,68,252,254,157,67,119,136,13,68,234,154,
159,67,79,72,13,68,96,149,161,67,108,129,182,7,68,106,166,205,67,98,6,182,7,68,62,171,205,67,14,181,7,68,16,176,205,67,147,180,7,68,228,180,205,67,99,109,0,128,7,68,216,25,180,67,108,33,12,10,68,178,242,159,67,98,237,167,10,68,176,37,155,67,195,32,13,
68,52,83,151,67,191,135,15,68,52,83,151,67,108,79,65,42,68,52,83,151,67,108,79,65,42,68,222,85,143,67,98,79,65,42,68,88,166,140,67,132,42,41,68,44,120,138,67,155,210,39,68,44,120,138,67,108,49,239,9,68,44,120,138,67,98,71,151,8,68,44,120,138,67,0,128,
7,68,88,166,140,67,0,128,7,68,222,85,143,67,108,0,128,7,68,216,25,180,67,99,109,227,139,56,68,70,121,143,67,98,227,139,56,68,148,199,142,67,103,98,56,68,4,32,142,67,251,27,56,68,16,180,141,67,98,196,149,54,68,154,95,139,67,26,123,49,68,252,146,131,67,
121,78,47,68,58,64,128,67,98,61,30,47,68,56,238,127,67,21,222,46,68,32,213,127,67,16,168,46,68,2,32,128,67,98,142,113,46,68,116,85,128,67,203,79,46,68,134,195,128,67,203,79,46,68,28,60,129,67,98,203,79,46,68,180,235,133,67,203,79,46,68,214,228,143,67,
203,79,46,68,38,207,147,67,98,203,79,46,68,114,213,148,67,146,140,46,68,202,204,149,67,194,244,46,68,248,107,150,67,98,37,200,48,68,56,54,153,67,134,248,53,68,136,35,161,67,98,218,55,68,142,4,164,67,98,169,251,55,68,176,55,164,67,1,41,56,68,94,64,164,
67,161,78,56,68,190,26,164,67,98,63,116,56,68,30,245,163,67,227,139,56,68,234,168,163,67,227,139,56,68,252,84,163,67,98,227,139,56,68,212,221,158,67,227,139,56,68,138,32,147,67,227,139,56,68,70,121,143,67,99,109,108,67,59,68,174,104,143,67,98,108,67,
59,68,138,140,142,67,10,119,59,68,216,188,141,67,87,206,59,68,36,55,141,67,98,138,139,61,68,216,142,138,67,160,250,66,68,14,65,130,67,9,219,68,68,112,198,126,67,98,232,249,68,68,180,103,126,67,223,35,69,68,128,87,126,67,153,70,69,68,148,156,126,67,98,
207,105,69,68,216,225,126,67,0,128,69,68,156,112,127,67,0,128,69,68,88,6,128,67,98,0,128,69,68,192,88,132,67,0,128,69,68,154,17,144,67,0,128,69,68,4,227,147,67,98,0,128,69,68,234,167,148,67,44,82,69,68,134,97,149,67,9,4,69,68,12,217,149,67,98,228,82,
67,68,160,110,152,67,51,176,61,68,106,11,161,67,111,217,59,68,30,218,163,67,98,117,189,59,68,134,5,164,67,90,151,59,68,60,13,164,67,133,119,59,68,104,237,163,67,98,175,87,59,68,138,206,163,67,108,67,59,68,240,140,163,67,108,67,59,68,132,70,163,67,98,
108,67,59,68,92,33,159,67,108,67,59,68,210,106,147,67,108,67,59,68,174,104,143,67,99,109,69,170,56,68,188,66,87,67,98,240,59,57,68,100,134,85,67,135,6,58,68,100,134,85,67,50,152,58,68,188,66,87,67,98,14,163,60,68,128,121,93,67,225,89,65,68,212,210,107,
67,131,38,67,68,136,76,113,67,98,237,81,67,68,224,208,113,67,126,107,67,68,32,159,114,67,126,107,67,68,188,121,115,67,98,126,107,67,68,132,84,116,67,237,81,67,68,148,34,117,67,131,38,67,68,32,167,117,67,98,67,79,65,68,56,65,123,67,158,106,60,68,214,18,
133,67,251,116,58,68,0,14,136,67,98,14,248,57,68,136,204,136,67,105,74,57,68,136,204,136,67,124,205,56,68,0,14,136,67,98,235,213,54,68,36,16,133,67,133,234,49,68,212,37,123,67,33,23,48,68,176,151,117,67,98,175,236,47,68,0,23,117,67,21,212,47,68,136,78,
116,67,21,212,47,68,188,121,115,67,98,21,212,47,68,28,165,114,67,175,236,47,68,168,220,113,67,33,23,48,68,196,91,113,67,98,232,223,49,68,216,237,107,67,124,157,54,68,76,127,93,67,69,170,56,68,188,66,87,67,99,101,0,0 };

				
				p.loadPathFromData(pathData, sizeof(pathData));

			}
			if (id == "template")
			{
				static const unsigned char pathData[] = { 110,109,153,180,7,68,16,238,204,67,108,215,173,7,68,178,47,205,67,108,233,171,7,68,138,79,205,67,98,254,141,7,68,26,22,207,67,26,35,8,68,92,118,208,67,159,13,9,68,92,118,208,67,108,50,122,40,68,92,118,208,67,98,136,119,41,68,92,118,208,67,54,121,42,68,
58,219,206,67,101,185,42,68,150,223,204,67,108,206,75,48,68,180,202,160,67,98,253,139,48,68,6,208,158,67,137,242,47,68,238,51,157,67,183,244,46,68,238,51,157,67,108,160,136,15,68,238,51,157,67,98,206,138,14,68,238,51,157,67,30,137,13,68,6,208,158,67,
241,72,13,68,180,202,160,67,108,135,182,7,68,150,223,204,67,98,12,182,7,68,106,228,204,67,20,181,7,68,64,233,204,67,153,180,7,68,16,238,204,67,99,109,0,128,7,68,58,80,179,67,108,103,12,10,68,216,39,159,67,98,69,168,10,68,88,89,154,67,96,33,13,68,6,135,
150,67,160,136,15,68,6,135,150,67,108,25,69,42,68,6,135,150,67,108,25,69,42,68,186,136,142,67,98,25,69,42,68,0,217,139,67,47,46,41,68,152,170,137,67,32,214,39,68,152,170,137,67,108,115,239,9,68,152,170,137,67,98,100,151,8,68,152,170,137,67,0,128,7,68,
0,217,139,67,0,128,7,68,186,136,142,67,108,0,128,7,68,58,80,179,67,99,109,254,245,68,68,234,164,135,67,98,0,128,69,68,122,145,134,67,0,128,69,68,212,210,132,67,254,245,68,68,100,191,131,67,98,16,120,66,68,124,134,125,67,28,224,59,68,216,38,99,67,127,
75,57,68,212,211,88,67,98,153,3,57,68,156,180,87,67,31,162,56,68,64,19,87,67,201,60,56,68,64,19,87,67,98,248,214,55,68,64,19,87,67,126,117,55,68,156,180,87,67,20,46,55,68,212,211,88,67,98,72,186,52,68,172,161,98,67,68,174,46,68,188,209,122,67,102,60,
44,68,46,77,130,67,98,197,167,43,68,160,118,131,67,197,167,43,68,250,88,133,67,102,60,44,68,108,130,134,67,98,151,206,46,68,150,167,139,67,44,99,53,68,166,208,152,67,123,228,55,68,44,211,157,67,98,10,40,56,68,80,89,158,67,194,130,56,68,152,164,158,67,
210,225,56,68,152,164,158,67,98,225,64,57,68,152,164,158,67,154,155,57,68,80,89,158,67,173,222,57,68,44,211,157,67,98,42,77,60,68,222,246,152,67,218,139,66,68,152,121,140,67,254,245,68,68,234,164,135,67,99,109,153,217,58,68,60,106,109,67,108,44,20,66,
68,94,42,133,67,108,168,145,64,68,212,46,136,67,108,126,60,63,68,212,46,136,67,108,225,35,63,68,170,95,136,67,108,193,11,63,68,212,46,136,67,108,187,116,60,68,212,46,136,67,108,187,116,60,68,108,194,144,67,108,229,36,57,68,232,97,151,67,108,229,36,57,
68,36,63,136,67,108,86,60,52,68,74,42,136,67,108,63,138,55,68,202,158,129,67,108,206,204,59,68,254,177,129,67,108,33,148,56,68,236,127,118,67,108,48,58,49,68,94,243,137,67,108,204,214,46,68,100,44,133,67,108,124,124,56,68,216,193,99,67,108,223,223,58,
68,200,79,109,67,108,153,217,58,68,60,106,109,67,99,101,0,0 };

				
				p.loadPathFromData(pathData, sizeof(pathData));
			}
			return p;
		}

		ProjectImporter& parent;

		HiseShapeButton newButton, importButton, templateButton;

		TextEditor folder;
		TextButton browse;
	};

	ScopedPointer<Header> header;

	ProjectImporter(BackendRootWindow* bpe_) :
		DialogWindowWithBackgroundThread("Create new HISE Project"),
		ControlledObject(bpe_->getMainController()),
		bpe(bpe_),
		ok(Result::ok())
	{
		addCustomComponent(header = new Header(*this));

		addBasicComponents(false);

		
		
		header->checkEnablement();
	};

	~ProjectImporter()
	{
		e = nullptr;
	}

	SourceType sourceType = SourceType::New;

	virtual void finished(URL::DownloadTask* task, bool success) override
	{
		showStatusMessage("Download finished");
		setProgress(1.0);
	}

	virtual void progress(URL::DownloadTask* task, int64 bytesDownloaded, int64 totalLength) override
	{
		auto x = (double)bytesDownloaded / (double)totalLength;

		setProgress(x);
	}

	void logStatusMessage(const String& message) override
	{
		debugToConsole(getMainController()->getMainSynthChain(), message);
		showStatusMessage(message);
	}

	void logVerboseMessage(const String& verboseMessage) 
	{
		debugToConsole(getMainController()->getMainSynthChain(), verboseMessage);
	};

	void criticalErrorOccured(const String& message) override
	{
		showStatusMessage(message);
	}

	void createSubDirectories()
	{
		logMessage("Create subdirectories");

		auto& handler = getMainController()->getSampleManager().getProjectHandler();
		auto ids = handler.getSubDirectoryIds();

		for (auto id : ids)
		{
			auto sub = newProjectFolder.getChildFile(handler.getIdentifier(id));
			sub.createDirectory();
		}
	}

	void extractPools()
	{
		logMessage("Extract files from Pools...");

		writePool<ValueTree>(e->pool->getSampleMapPool(), [&](File outputFile, const ValueTree& data, const var&)
		{
			logMessage("Write samplemap to " + outputFile.getFullPathName());
			auto xml = data.createXml();
			outputFile.replaceWithText(xml->createDocument(""));
		});

		writePool<Image>(e->pool->getImagePool(), [&](File outputFile, const Image& img, const var&)
		{
			logMessage("Write image to " + outputFile.getFullPathName());

			if (auto format = ImageFileFormat::findImageFormatForFileExtension(outputFile))
			{
				FileOutputStream fos(outputFile);
				outputFile.getParentDirectory().createDirectory();
				format->writeImageToStream(img, fos);
			}
		});

		writePool<AudioSampleBuffer>(e->pool->getAudioSampleBufferPool(), [&](File outputFile, const AudioSampleBuffer& buffer, const var& additionalData)
		{
			logMessage("Write audio file to " + outputFile.getFullPathName());

			OwnedArray<AudioFormat> formats;

			formats.add(new WavAudioFormat());
			formats.add(new AiffAudioFormat());
			formats.add(new juce::OggVorbisAudioFormat());

			for (auto af : formats)
			{
				if (af->getFileExtensions().contains(outputFile.getFileExtension()))
				{
					auto fos = new FileOutputStream(outputFile);
					outputFile.getParentDirectory().createDirectory();

					auto sr = (int)additionalData["SampleRate"];

					AudioChannelSet channels;
					ScopedPointer<AudioFormatWriter> writer = af->createWriterFor(fos, sr, buffer.getNumChannels(), 24, {}, 5);

					writer->writeFromAudioSampleBuffer(buffer, 0, buffer.getNumSamples());
					writer->flush();
					writer = nullptr;
					return;
				}
			}
		});

		writePool<MidiFileReference>(e->pool->getMidiFilePool(), [&](File output, const MidiFileReference& mf, const var& data)
		{
			logMessage("Write MIDI file " + output.getFullPathName());

			output.deleteFile();
			FileOutputStream fos(output);
			mf.getFile().writeTo(fos);
		});
	}

	void extractFonts()
	{
		logMessage("Extract fonts");

		auto imageRoot = newProjectFolder.getChildFile(ProjectHandler::getIdentifier(FileHandlerBase::Images));

		for (auto f : getMainController()->exportCustomFontsAsValueTree())
		{
			auto fileName = f["Name"].toString().fromFirstOccurrenceOf("}", false, false);

			auto outputFile = imageRoot.getChildFile(fileName);
			outputFile.getParentDirectory().createDirectory();

			logMessage("Write font " + outputFile.getFullPathName());

			auto size = (int)f["Size"];
			
			auto id = f["FontId"].toString();

			if (auto data = f["Data"].getBinaryData())
			{
				outputFile.deleteFile();
				FileOutputStream fos(outputFile);
				fos.write(data->getData(), size);
				fos.flush();
			}
		}
	}

	void extractNetworks()
	{
		logMessage("Extract networks...");

		auto networkFolder = newProjectFolder.getChildFile(ProjectHandler::getIdentifier(FileHandlerBase::DspNetworks)).getChildFile("Networks");
		networkFolder.createDirectory();

		for (auto n : e->networks)
		{

			auto xml = n.createXml();
			auto xmlText = xml->createDocument("");

			auto filename = n[scriptnode::PropertyIds::ID].toString();

			auto outputFile = networkFolder.getChildFile(filename).withFileExtension("xml");
			outputFile.getParentDirectory().createDirectory();

			logMessage("Write DSP network " + outputFile.getFullPathName());
			outputFile.replaceWithText(xmlText);
		}
	}

	void extractScripts()
	{
		logMessage("Extract scripts");

		auto scriptRoot = newProjectFolder.getChildFile(ProjectHandler::getIdentifier(FileHandlerBase::Scripts));

		valuetree::Helpers::forEach(e->presetToLoad, [&](ValueTree& v)
		{
			if (v.hasProperty("Script"))
			{
				auto code = v["Script"].toString();

				auto includedFiles = JavascriptProcessor::Helpers::desolveIncludeStatements(code, scriptRoot, getMainController());

				for (auto f : includedFiles)
				{
					logMessage("Extract script " + f.f.getFullPathName());
					f.f.getParentDirectory().createDirectory();
					f.f.replaceWithText(f.content);
				}

				v.setProperty("Script", code, nullptr);
			}
			return false;
		});

		
	}

	void createProjectSettings()
	{
		auto allData = e->getValueTreeFromFile(e->getExpansionType());
		
		auto iconData = allData.getChildWithName(ExpansionIds::HeaderData).getChildWithName(ExpansionIds::Icon)[ExpansionIds::Data].toString();

		if (iconData.isNotEmpty())
		{
			logMessage("Write Icon.png image file");
			MemoryBlock mb;
			mb.fromBase64Encoding(iconData);

			auto output = newProjectFolder.getChildFile(ProjectHandler::getIdentifier(FileHandlerBase::Images)).getChildFile("Icon.png");

			juce::PNGImageFormat format;
			auto icon = format.loadFrom(mb.getData(), mb.getSize());

			FileOutputStream fos(output);

			format.writeImageToStream(icon, fos);
		}

		logMessage("Create project setting files");

		auto headerData = allData.getChildWithName(ExpansionIds::ExpansionInfo).createCopy();

		auto projectSettings = headerData.getChildWithName("ProjectSettings");

		if (!projectSettings.isValid())
			projectSettings = ValueTree("ProjectSettings");

		auto userSettings = headerData.getChildWithName("UserSettings");

		if (!userSettings.isValid())
			userSettings = ValueTree("UserSettings");

		std::map<Identifier, ValueTree> map;
		map[HiseSettings::SettingFiles::ProjectSettings] = projectSettings;
		map[HiseSettings::SettingFiles::UserSettings] = userSettings;
		map[HiseSettings::SettingFiles::ExpansionSettings] = ValueTree("ExpansionInfo");

		std::map<Identifier, File> fileMap;
		fileMap[HiseSettings::SettingFiles::ProjectSettings] = newProjectFolder.getChildFile("project_info.xml");
		fileMap[HiseSettings::SettingFiles::UserSettings] = newProjectFolder.getChildFile("user_info.xml");
		fileMap[HiseSettings::SettingFiles::ExpansionSettings] = newProjectFolder.getChildFile("expansion_info.xml");

		auto writeSetting = [&map](const Identifier& target, const Identifier& p, const String& value)
		{
			ValueTree c(p);
			c.setProperty("value", value, nullptr);
			map[target].addChild(c, -1, nullptr);
		};

		auto saveSetting = [&map, &fileMap](const Identifier& id)
		{
			auto xml = map[id].createXml();
			auto xmlText = xml->createDocument("");
			auto f = fileMap[id];
			f.replaceWithText(xmlText);
		};

		if (projectSettings.getNumChildren() == 0)
		{
			writeSetting(HiseSettings::SettingFiles::ProjectSettings, HiseSettings::Project::Name, headerData["Name"]);
			writeSetting(HiseSettings::SettingFiles::ProjectSettings, HiseSettings::Project::Version, headerData["Version"]);
			writeSetting(HiseSettings::SettingFiles::ProjectSettings, HiseSettings::Project::EncryptionKey, "1234");
		}
		
		if (userSettings.getNumChildren() == 0)
		{
			writeSetting(HiseSettings::SettingFiles::UserSettings, HiseSettings::User::Company, headerData["Company"]);
			writeSetting(HiseSettings::SettingFiles::UserSettings, HiseSettings::User::CompanyURL, headerData["CompanyURL"]);
		}
		
		writeSetting(HiseSettings::SettingFiles::ExpansionSettings, HiseSettings::ExpansionSettings::Description, headerData["Description"]);
		writeSetting(HiseSettings::SettingFiles::ExpansionSettings, HiseSettings::ExpansionSettings::Tags, headerData["Tags"]);
		writeSetting(HiseSettings::SettingFiles::ExpansionSettings, HiseSettings::ExpansionSettings::UUID, headerData["UUID"]);
		
		saveSetting(HiseSettings::SettingFiles::ProjectSettings);
		saveSetting(HiseSettings::SettingFiles::UserSettings);
		saveSetting(HiseSettings::SettingFiles::ExpansionSettings);
	}

	void extractPreset()
	{
		logMessage("Extract main preset");
		auto xml = e->presetToLoad.createXml();

		auto xmlFolder = e->getRootFolder().getChildFile(e->getIdentifier(FileHandlerBase::Presets));

		FileOutputStream fos(xmlFolder.getChildFile("Preset.hip"));

		e->presetToLoad.writeToStream(fos);
	}

	void extractUserPresets()
	{
		auto allData = e->getValueTreeFromFile(e->getExpansionType());

		logMessage("Extracting user presets...");
		e->extractUserPresetsIfEmpty(allData, true);
	}

	void extractWebResources()
	{
		for (auto id : getMainController()->getAllWebViewIds())
		{
			auto wv = getMainController()->getOrCreateWebView(id);
			auto ok = wv->explode();
            ignoreUnused(ok);
		}
		
	}

	void logMessage(const String& message)
	{
		showStatusMessage(message);
		DBG(message);
		debugToConsole(getMainController()->getMainSynthChain(), message);
	}

	void run() override
	{
		jassert(newProjectFolder.isDirectory());
		jassert(newProjectFolder.getNumberOfChildFiles(File::findFilesAndDirectories) <= 1);

		
		Array<File> sampleArchives;

		auto hxiFile = newProjectFolder.getChildFile("info.hxi");
		
		if (sourceType == SourceType::Template)
		{
			// Download the template from Github...

			URL d("https://docs.hise.audio/info.hxi");

			URL::DownloadTaskOptions options;
			options.listener = this;

			showStatusMessage("Download Template");
			setProgress(0.0);

			auto task = d.downloadToFile(hxiFile, options);

			while (!task->isFinished())
				wait(500);
		}
		
		if(sourceType == SourceType::Import)
		{
			auto archive = header->importFile;

			if (archive.getFileExtension() == ".hxi")
				archive.copyFileTo(hxiFile);
			else if (archive.getFileExtension() == ".hiseproject")
			{
				FileInputStream fis(archive);

				hxiFile.deleteFile();

				FileOutputStream fos(hxiFile);

				auto numHeaderBytes = fis.readInt64();

				fos.writeFromInputStream(fis, numHeaderBytes);

				int archiveIndex = 0;

				while (!fis.isExhausted())
				{
					auto numBytes = fis.readInt64();

					FileOutputStream so(hxiFile.getSiblingFile("Samples" + String(++archiveIndex)).withFileExtension(".hr1"));
					so.writeFromInputStream(fis, numBytes);

					sampleArchives.add(so.getFile());
				}
			}
			else
			{
				ZipFile zf(archive);
				zf.uncompressTo(newProjectFolder);
			}
		}

		if (sourceType == SourceType::New)
		{
			GET_PROJECT_HANDLER(getMainController()->getMainSynthChain()).createNewProject(newProjectFolder, this);
			return;
		}

		auto key = "1234";

		getMainController()->getExpansionHandler().setExpansionType<FullInstrumentExpansion>();
		getMainController()->getExpansionHandler().setEncryptionKey(key);
		
		e = new FullInstrumentExpansion(getMainController(), newProjectFolder);

		createSubDirectories();
		

		getMainController()->setWebViewRoot(newProjectFolder);

		ok = e->initialise();

		if (ok.failed())
			return;

		ok = e->lazyLoad();

		if (ok.failed())
			return;

		extractFonts();

		createProjectSettings();
		extractScripts();
		extractPreset();
		extractPools();
		extractNetworks();
		extractUserPresets();
		extractWebResources();

		for (auto s : sampleArchives)
		{
			showStatusMessage("Extract Sample Archive " + s.getFileName());

			hlac::HlacArchiver::DecompressData data;

			data.option = hlac::HlacArchiver::OverwriteOption::ForceOverwrite;

			data.supportFullDynamics = true;
			data.sourceFile = s;
			data.targetDirectory = e->getSubDirectory(FileHandlerBase::Samples);
			data.progress = &logData.progress;
			data.partProgress = &logData.progress;
			data.totalProgress = &getProgressCounter();

			hlac::HlacArchiver decompressor(getCurrentThread());

			decompressor.setListener(this);

			bool ok = decompressor.extractSampleData(data);
            ignoreUnused(ok);

		}

		logMessage("... Done!");
	}

	template <typename DataType, typename PoolType> void writePool(PoolType& pool, const std::function<void(File, const DataType&, const var&)>& fileOp)
	{
		auto id = pool.getFileTypeName();

		logMessage("Extract " + id + " Pool...");

		auto type = FileHandlerBase::getSubDirectoryForIdentifier(id);
		auto poolRoot = e->getRootFolder().getChildFile(e->getIdentifier(type));
		pool.loadAllFilesFromDataProvider();

		for (int i = 0; i < pool.getNumLoadedFiles(); i++)
		{
			if (auto ptr = pool.loadFromReference(pool.getReference(i), PoolHelpers::LoadAndCacheWeak))
			{
				setProgress((double)i / (double)pool.getNumLoadedFiles());

				auto target = ptr->ref.resolveFile(e, type);

				target.getParentDirectory().createDirectory();
				target.deleteFile();

				fileOp(target, ptr->data, ptr->additionalData);
			}
		}
	}

	void threadFinished() override
	{
		if (ok.failed())
			PresetHandler::showMessageWindow("Error importing project", ok.getErrorMessage(), PresetHandler::IconType::Error);

		if(newProjectFolder.isDirectory())
			getMainController()->getSampleManager().getProjectHandler().setWorkingProject(newProjectFolder, true);

		
		dynamic_cast<GlobalSettingManager*>(getMainController())->getSettingsObject().refreshProjectData();

		if (sourceType == SourceType::New)
		{
			bpe->mainEditor->clearPreset();
		}
		else
		{
			auto presetToLoad = newProjectFolder.getChildFile(ProjectHandler::getIdentifier(FileHandlerBase::Presets)).getChildFile("Preset.hip");
			bpe->loadNewContainer(presetToLoad);
		}
	}


	BackendRootWindow* bpe;

	

	File newProjectFolder;
	
	ScopedPointer<FullInstrumentExpansion> e = nullptr;

	Result ok;
};




} // namespace hise