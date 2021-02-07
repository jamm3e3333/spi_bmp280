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
    temperature : real {init_value = 0};
    Data_visible : boolean;
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

  label label_7;
    gui
      owner = panel_1;
      position = 545, 98;
    end_gui;
    text_list
      font = 'Tahoma', 18, normal;
      text = 'Zapni měření teploty';
    end_text_list;
  end_label;

  switch switch_2;
    gui
      owner = panel_1;
      position = 630, 150, 65, 65;
      window
        disable = zoom, maximize;
      end_window;
    end_gui;
    output = Data_visible;
  end_switch;

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
      visibility = Data_visible;
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
    CH_vystup = 'sZ';
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

  program program_Prijem;
    static
      var1 : integer {init_value = 0};
      var2 : integer {init_value = 0};
      t_fine : real {init_value = 0};
      T : real {init_value = 0};
    end_static;

    activity
      period = 1;
      driver = ASCII;
    end_activity;

    procedure OnActivate();
    begin
    
    V_vstup = CH_vstup;
    temperature = val(V_vstup,10);
    
    var1 = floor(bitshr(( ((bitshr(temperature,3)) - (bitshl(30463,1)) )*(30463)),11));
    var2 = floor(bitshr(( ( ((bitshr(temperature,4))-(30463))*bitshr(((bitshr(temperature,4))-(30463)),12)) *(30463)),14) );
    t_fine = var1 + var2;
    T = ((t_fine*5)+128)/5120.0;
    T = round2(T*0.4,2);
    V_vstup = str(T,10);
    
    
    send string_display_3;
    end_procedure;

  end_program;

end_instrument;

