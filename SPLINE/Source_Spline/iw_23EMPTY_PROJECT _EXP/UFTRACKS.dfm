object fTrack: TfTrack
  Left = 138
  Top = 193
  Width = 776
  Height = 561
  Caption = #1059#1095#1072#1089#1090#1086#1082
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  OnCreate = FormCreate
  PixelsPerInch = 96
  TextHeight = 13
  object Panel1: TPanel
    Left = 0
    Top = 0
    Width = 760
    Height = 33
    Align = alTop
    TabOrder = 0
    object sbAssemTrace: TSpeedButton
      Left = 26
      Top = 0
      Width = 221
      Height = 33
      AllowAllUp = True
      GroupIndex = 1
      Caption = ' '#1050#1086#1084#1087#1086#1085#1086#1074#1082#1072', '#1072#1085#1072#1083#1080#1079'   '#1080' '#1079#1072#1087#1080#1089#1100' '#1091#1095#1072#1089#1090#1082#1072
      OnClick = sbAssemTraceClick
    end
    object Button2: TButton
      Left = 247
      Top = 0
      Width = 145
      Height = 33
      Caption = #1050#1086#1087#1080#1088#1086#1074#1072#1085#1080#1077' '#1091#1095#1072#1089#1090#1082#1072
      TabOrder = 0
      OnClick = Button2Click
    end
    object Button3: TButton
      Left = 391
      Top = 0
      Width = 121
      Height = 33
      Caption = #1054#1095#1080#1089#1090#1082#1072' '#1091#1095#1072#1089#1090#1082#1072
      TabOrder = 1
      OnClick = Button3Click
    end
    object Button4: TButton
      Left = 583
      Top = 0
      Width = 151
      Height = 33
      Caption = ' '#1047#1072#1087#1080#1089#1100' '#1091#1075#1083#1086#1075#1088#1072#1084#1084#1099' '#1080'  '#1061','#1059
      TabOrder = 2
      Visible = False
      OnClick = Button4Click
    end
    object Button1: TButton
      Left = 510
      Top = -1
      Width = 75
      Height = 33
      Caption = #1042#1086#1079#1074#1088#1072#1090
      TabOrder = 3
      OnClick = Button1Click
    end
  end
  object Panel2: TPanel
    Left = 0
    Top = 170
    Width = 760
    Height = 352
    Align = alClient
    TabOrder = 1
    object sgTrack: TStringGrid
      Left = 9
      Top = 0
      Width = 632
      Height = 353
      ColCount = 3
      DefaultColWidth = 210
      FixedCols = 0
      RowCount = 2
      TabOrder = 0
    end
  end
  object Panel3: TPanel
    Left = 0
    Top = 33
    Width = 760
    Height = 137
    Align = alTop
    TabOrder = 2
    object Label1: TLabel
      Left = 552
      Top = 36
      Width = 137
      Height = 22
      AutoSize = False
      Caption = '    MAX. '#1089#1076#1074#1080#1078#1082#1072
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -15
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
    end
    object GroupBox2: TGroupBox
      Left = 240
      Top = 17
      Width = 257
      Height = 112
      TabOrder = 0
      object Label3: TLabel
        Left = 7
        Top = 48
        Width = 96
        Height = 13
        Caption = #1050#1086#1085#1077#1095#1085#1099#1081' '#1087#1080#1082#1077#1090#1072#1078
      end
      object Label2: TLabel
        Left = 8
        Top = 16
        Width = 103
        Height = 13
        Caption = #1053#1072#1095#1072#1083#1100#1085#1099#1081' '#1087#1080#1082#1077#1090#1072#1078
      end
      object Edit1: TEdit
        Left = 114
        Top = 16
        Width = 134
        Height = 21
        TabOrder = 0
        Text = '                  +      '
        OnClick = Edit1Click
        OnExit = Edit1Exit
      end
      object Edit2: TEdit
        Left = 113
        Top = 48
        Width = 131
        Height = 21
        TabOrder = 1
        Text = '                   +       '
        OnClick = Edit2Click
        OnExit = Edit2Exit
      end
      object Button8: TButton
        Left = 30
        Top = 83
        Width = 75
        Height = 25
        Caption = #1047#1072#1087#1080#1089#1072#1090#1100
        TabOrder = 2
        OnClick = Button8Click
      end
      object Button9: TButton
        Left = 144
        Top = 84
        Width = 75
        Height = 22
        Caption = #1042#1086#1079#1074#1088#1072#1090
        TabOrder = 3
        OnClick = Button9Click
      end
    end
    object GroupBox1: TGroupBox
      Left = 24
      Top = 0
      Width = 169
      Height = 123
      TabOrder = 1
      object Button5: TButton
        Left = 8
        Top = 36
        Width = 137
        Height = 25
        Caption = #1042#1082#1083#1102#1095#1080#1090#1100' '#1082#1088#1080#1074#1091#1102
        TabOrder = 0
        OnClick = Button5Click
      end
      object Button6: TButton
        Left = 8
        Top = 72
        Width = 137
        Height = 25
        Caption = #1047#1072#1087#1080#1089#1100' '#1091#1095#1072#1089#1090#1082#1072
        TabOrder = 1
        OnClick = Button6Click
      end
    end
    object Edit3: TEdit
      Left = 572
      Top = 58
      Width = 69
      Height = 21
      AutoSize = False
      TabOrder = 2
      Text = '  2.0'
    end
  end
end
