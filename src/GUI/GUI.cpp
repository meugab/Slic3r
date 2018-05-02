#include <wx/wxprec.h>
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif
#include <wx/display.h>
#include <wx/filefn.h> 
#include <string>


#include "MainFrame.hpp"
#include "GUI.hpp"
#include "misc_ui.hpp"
#include "Preset.hpp"

// Logging mechanism
#include "Log.hpp"

namespace Slic3r { namespace GUI {

/// Primary initialization and point of entry into the GUI application.
/// Calls MainFrame and handles preset loading, etc.
bool App::OnInit()
{
    this->SetAppName("Slic3r");
    // TODO: Call a logging function with channel GUI, severity info

    this->notifier = std::unique_ptr<Notifier>();

    datadir = decode_path(wxStandardPaths::Get().GetUserDataDir());
    wxString enc_datadir = encode_path(datadir);

    const wxString& slic3r_ini  {datadir + "/slic3r.ini"};
    const wxString& print_ini   {datadir + "/print"};
    const wxString& printer_ini {datadir + "/printer"};
    const wxString& material_ini {datadir + "/filament"};

    // if we don't have a datadir or a slic3r.ini, prompt for wizard.
    bool run_wizard = (wxDirExists(datadir) && wxFileExists(slic3r_ini));
    
    /* Check to make sure if datadir exists */
    for (auto& dir : std::vector<wxString> { enc_datadir, print_ini, printer_ini, material_ini }) {
        if (wxDirExists(dir)) continue;
        if (!wxMkdir(dir)) {
            Slic3r::Log::fatal_error(LogChannel, (_("Slic3r was unable to create its data directory at ")+ dir).ToStdWstring());
        }
    }

    // TODO: Call a logging function with channel GUI, severity info for datadir path
    Slic3r::Log::info(LogChannel, (_("Data dir: ") + datadir).ToStdWstring());

    // Load gui settings from slic3r.ini
    if (wxFileExists(slic3r_ini)) {
    /*
        my $ini = eval { Slic3r::Config->read_ini("$datadir/slic3r.ini") };
        if ($ini) {
            $last_version = $ini->{_}{version};
            $ini->{_}{$_} = $Settings->{_}{$_}
                for grep !exists $ini->{_}{$_}, keys %{$Settings->{_}};
            $Settings = $ini;
        }
        delete $Settings->{_}{mode};  # handle legacy
    */
    }


    this->gui_config->save_settings();

    // Load presets
    this->load_presets();


    wxImage::AddHandler(new wxPNGHandler());
    MainFrame *frame = new MainFrame( "Slic3r", wxDefaultPosition, wxDefaultSize, this->gui_config);
    this->SetTopWindow(frame);

    // Load init bundle
    //

    // Run the wizard if we don't have an initial config
    /*
    $self->check_version
        if $self->have_version_check
            && ($Settings->{_}{version_check} // 1)
            && (!$Settings->{_}{last_version_check} || (time - $Settings->{_}{last_version_check}) >= 86400);
    */

    // run callback functions during idle on the main frame
    /*
    EVT_IDLE($frame, sub {
        while (my $cb = shift @cb) {
            $cb->();
        }
    });
    */

    // Handle custom version check event
    /*
    EVT_COMMAND($self, -1, $VERSION_CHECK_EVENT, sub {
        my ($self, $event) = @_;
        my ($success, $response, $manual_check) = @{$event->GetData};
        
        if ($success) {
            if ($response =~ /^obsolete ?= ?([a-z0-9.-]+,)*\Q$Slic3r::VERSION\E(?:,|$)/) {
                my $res = Wx::MessageDialog->new(undef, "A new version is available. Do you want to open the Slic3r website now?",
                    'Update', wxYES_NO | wxCANCEL | wxYES_DEFAULT | wxICON_INFORMATION | wxICON_ERROR)->ShowModal;
                Wx::LaunchDefaultBrowser('http://slic3r.org/') if $res == wxID_YES;
            } else {
                Slic3r::GUI::show_info(undef, "You're using the latest version. No updates are available.") if $manual_check;
            }
            $Settings->{_}{last_version_check} = time();
            $self->save_settings;
        } else {
            Slic3r::GUI::show_error(undef, "Failed to check for updates. Try later.") if $manual_check;
        }
    });
    */

    return true;
}

void App::save_window_pos(const wxTopLevelWindow* window, const wxString& name ) {
    this->gui_config->window_pos[name] = 
        std::make_tuple<wxPoint, wxSize, bool>(
            window->GetScreenPosition(),
            window->GetSize(),
            window->IsMaximized());

    this->gui_config->save_settings();
}

void App::restore_window_pos(wxTopLevelWindow* window, const wxString& name ) {
    try {
        auto tmp = gui_config->window_pos[name];
        const auto& size = std::get<1>(tmp);
        const auto& pos = std::get<0>(tmp);
        window->SetSize(size);

        auto display = wxDisplay().GetClientArea();
        if (((pos.x + size.x / 2) < display.GetRight()) && (pos.y + size.y/2 < display.GetBottom()))
            window->Move(pos);

        window->Maximize(std::get<2>(tmp));
    }
    catch (std::out_of_range e) {
        // config was empty
    }
}

void App::load_presets() {
/*
    for my $group (qw(printer filament print)) {
        my $presets = $self->{presets}{$group};
        
        # keep external or dirty presets
        @$presets = grep { ($_->external && $_->file_exists) || $_->dirty } @$presets;
        
        my $dir = "$Slic3r::GUI::datadir/$group";
        opendir my $dh, Slic3r::encode_path($dir)
            or die "Failed to read directory $dir (errno: $!)\n";
        foreach my $file (grep /\.ini$/i, readdir $dh) {
            $file = Slic3r::decode_path($file);
            my $name = basename($file);
            $name =~ s/\.ini$//i;
            
            # skip if we already have it
            next if any { $_->name eq $name } @$presets;
            
            push @$presets, Slic3r::GUI::Preset->new(
                group   => $group,
                name    => $name,
                file    => "$dir/$file",
            );
        }
        closedir $dh;
    
        @$presets = sort { $a->name cmp $b->name } @$presets;
    
        unshift @$presets, Slic3r::GUI::Preset->new(
            group   => $group,
            default => 1,
            name    => '- default -',
        );
    }
*/
}

}} // namespace Slic3r::GUI