**Usage**: `IntelArcSync_App.exe [option] [sub-option]`

A sample app to control Intel Arc Sync profile options.

**Options**
<pre>
get
        caps                    Print Intel Arc Sync caps
        profile                 Print active Intel Arc Sync profile details
        profile-names           Print the list of profile names available

set
        profile                 Set the Intel Arc Sync profile
                < profile-name >          Name of Intel Arc Sync profile to apply
                [< min-rr >]              Used only in CUSTOM profile. Min RR value in Hz.
                [< max-rr >]              Used only in CUSTOM profile. Max RR value in Hz.
                [< sfdit-value >]         Used only in CUSTOM profile. SFDIT value in micro seconds.
                [< sfddt-value >]         Used only in CUSTOM profile. SFDDT value in micro seconds.
</pre>

**Examples**:

IntelArcSync_App.exe get caps

IntelArcSync_App.exe get profile

IntelArcSync_App.exe get profile-names

IntelArcSync_App.exe set profile RECOMMENDED

IntelArcSync_App.exe set profile OFF

IntelArcSync_App.exe set profile EXCELLENT

IntelArcSync_App.exe set profile CUSTOM 60 90 5000 5000
