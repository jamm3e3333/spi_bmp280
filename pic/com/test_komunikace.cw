directories
end_directories;

settings
  operation_mode = real_time;
  startup_options
    call_procedures = false;
    activate_receivers = false;
    output_action = set_local;
  end_startup_options;
end_settings;


driver
  ASCII {driver = 'ascdrv5.dll'; map_file = 'sendmail.DMF'; parameter_file = 'Ascdrv5.par'};
end_driver;

data

  var
    V_vstup : string;
    V_vystup : string;
    V_PWM : integer {init_value = 0};
  end_var;

  channel
    CH_vstup : string {driver = ASCII; driver_index = 20; direction = input};
    CH_vystup : string {driver = ASCII; driver_index = 22; direction = output};
    CH_vyjimka : boolean {driver = ASCII; driver_index = 2; direction = output};
  end_channel;

end_data;

instrument

  panel panel_1;
    gui
      owner = background;
      position = 70, 105, 825, 515;
      window
        type = tool;
        title = 'com_terminal';
      end_window;
    end_gui;

    procedure OnWindowClose();
    begin
      core.StopApplication();
    end_procedure;

  end_panel;

  label label_4;
    gui
      owner = panel_1;
      position = 345, 100;
    end_gui;
    text_list
      font = 'Trebuchet MS', 17, normal;
      text = 'Teplota [°C]';
    end_text_list;
  end_label;

  string_display string_display_3;
    gui
      owner = panel_1;
      position = 322, 170, 190, 135;
    end_gui;
    expression = V_vstup;
    blink_rate = quick;
    font = 'Tahoma', 20, normal;
    justify = center;
  end_string_display;

  program program_Vysilani;

    procedure OnActivate();
    begin
    CH_vyjimka = false;
    CH_vystup = 'sA';
    end_procedure;

  end_program;

  program program_Prijem;
    activity
      driver = ASCII;
    end_activity;

    procedure OnActivate();
    begin
    V_vstup = CH_vstup;
    send string_display_3;
    end_procedure;

  end_program;

  program program_Vyjimka;
    activity
      period = 1;
    end_activity;

    procedure OnActivate();
    begin
    CH_vyjimka = true;
    send program_Vysilani;
    end_procedure;

  end_program;

end_instrument;

