object Form1: TForm1
  Left = 17
  Top = 125
  Width = 969
  Height = 605
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -14
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 16
  object TPaintBox
    Left = 378
    Top = 85
    Width = 104
    Height = 104
  end
  object MainBox: TScrollBox
    Left = 0
    Top = 0
    Width = 953
    Height = 983
    Align = alTop
    Anchors = [akTop, akRight, akBottom]
    TabOrder = 0
    object Fild: TPaintBox
      Left = -18
      Top = 8
      Width = 943
      Height = 529
      Color = clBtnHighlight
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -12
      Font.Name = 'Arial'
      Font.Style = []
      ParentColor = False
      ParentFont = False
      OnClick = FildClick
      OnDblClick = FildDblClick
      OnMouseDown = FildMouseDown
      OnMouseMove = FildMouseMove
      OnMouseUp = FildMouseUp
      OnPaint = FildPaint
    end
    object Label2: TLabel
      Left = 48
      Top = 264
      Width = 177
      Height = 28
      AutoSize = False
      Caption = '       '#1061#1086#1076'  '#1087#1088#1086#1094#1077#1089#1089#1072
      Color = clLime
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -16
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentColor = False
      ParentFont = False
      Visible = False
    end
    object PRB: TProgressBar
      Left = 46
      Top = 296
      Width = 178
      Height = 28
      Min = 0
      Max = 100
      TabOrder = 0
      Visible = False
    end
  end
  object Panel1: TPanel
    Left = -8
    Top = 328
    Width = 954
    Height = 65
    Alignment = taLeftJustify
    TabOrder = 1
    object Label1: TLabel
      Left = 16
      Top = 8
      Width = 88
      Height = 16
      Caption = #1052#1072#1089#1096#1090#1072#1073' '#1087#1086' '#1061
    end
    object Label4: TLabel
      Left = 166
      Top = 8
      Width = 89
      Height = 16
      Caption = #1052#1072#1089#1096#1090#1072#1073' '#1087#1086' Y'
    end
    object ButCenter: TSpeedButton
      Left = 315
      Top = 8
      Width = 56
      Height = 25
      AllowAllUp = True
      GroupIndex = 1
      Down = True
      Caption = #1057#1076#1074#1080#1075
      OnClick = ButCenterClick
    end
    object btnDelete: TButton
      Left = 513
      Top = 42
      Width = 148
      Height = 22
      Caption = #1059#1076#1072#1083#1080#1090#1100' '#1090#1086#1095#1082#1080
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -16
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
      TabOrder = 0
      Visible = False
      OnClick = btnDeleteClick
    end
    object btnSave: TButton
      Left = 625
      Top = 8
      Width = 120
      Height = 25
      Caption = ' '#1042#1099#1093#1086#1076' '#1089' '#1079#1072#1087#1080#1089#1100#1102
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -13
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ModalResult = 1
      ParentFont = False
      TabOrder = 1
      OnClick = btnSaveClick
    end
    object btnRef: TButton
      Left = 40
      Top = 38
      Width = 108
      Height = 25
      Caption = #1048#1085#1089#1090#1088#1091#1082#1094#1080#1103
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -16
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
      TabOrder = 2
      OnClick = btnRefClick
    end
    object SizeX: TEdit
      Left = 104
      Top = 8
      Width = 45
      Height = 24
      ReadOnly = True
      TabOrder = 3
      Text = '0'
    end
    object ButSizeX: TUpDown
      Left = 148
      Top = 8
      Width = 16
      Height = 25
      Min = -10000
      Max = 10000
      Position = 0
      TabOrder = 4
      Wrap = False
      OnClick = ButSizeXClick
    end
    object SizeY: TEdit
      Left = 256
      Top = 8
      Width = 45
      Height = 24
      ReadOnly = True
      TabOrder = 5
      Text = '0'
    end
    object ButSizeY: TUpDown
      Left = 300
      Top = 8
      Width = 17
      Height = 25
      Min = -10000
      Max = 10000
      Increment = 10
      Position = 0
      TabOrder = 6
      Wrap = False
      OnClick = ButSizeYClick
    end
    object Button2: TButton
      Left = 662
      Top = 43
      Width = 158
      Height = 22
      Caption = ' '#1057#1084#1077#1085#1072'  '#1088#1077#1078#1080#1084#1072' , '#1074#1099#1093#1086#1076
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -13
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
      TabOrder = 7
      OnClick = Button2Click
    end
    object ButCancel: TButton
      Left = 373
      Top = 8
      Width = 67
      Height = 25
      Caption = #1042' '#1085#1072#1095#1072#1083#1086
      TabOrder = 8
      OnClick = ButCancelClick
    end
    object Button3: TButton
      Left = 517
      Top = 11
      Width = 109
      Height = 23
      Hint = #1056#1072#1089#1095#1105#1090' '#1090#1088#1072#1089#1089#1099' '#1087#1086' '#1090#1086#1095#1082#1072#1084' '#1085#1072' '#1087#1088#1103#1084#1099#1093
      Caption = #1056#1072#1089#1089#1095#1080#1090#1072#1090#1100
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -19
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
      TabOrder = 9
      OnClick = Button3Click
    end
    object Edit1: TEdit
      Left = 375
      Top = 39
      Width = 135
      Height = 24
      TabOrder = 10
    end
    object CheckBox1: TCheckBox
      Left = 205
      Top = 39
      Width = 168
      Height = 31
      Caption = #1055#1086#1082#1072#1079#1072#1090#1100' '#1088#1072#1076#1080#1091#1089
      Checked = True
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -16
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
      State = cbChecked
      TabOrder = 11
      OnClick = CheckBox1Click
    end
    object Button1: TButton
      Left = 440
      Top = 10
      Width = 75
      Height = 23
      Caption = #1042' '#1082#1086#1085#1077#1094
      TabOrder = 12
      OnClick = Button1Click
    end
  end
end
