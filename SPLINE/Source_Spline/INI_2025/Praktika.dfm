object Form1: TForm1
  Left = 40
  Top = 186
  Width = 916
  Height = 620
  Caption = '  '#1042#1080#1079#1091#1072#1083#1080#1079#1072#1094#1080#1103' '#1080#1089#1093#1086#1076#1085#1086#1081' '#1090#1088#1072#1089#1089#1099' : '#1087#1086' '#1082#1086#1086#1088#1076#1080#1085#1072#1090#1072#1084'  '#1080#1083#1080' '#1091#1075#1083#1086#1075#1088#1072#1084#1084#1086#1081
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
    Left = 370
    Top = 85
    Width = 104
    Height = 104
  end
  object MainBox: TScrollBox
    Left = 0
    Top = 0
    Width = 900
    Height = 1016
    Align = alTop
    Anchors = [akTop, akRight, akBottom]
    TabOrder = 0
    object Fild: TPaintBox
      Left = 12
      Top = 0
      Width = 878
      Height = 427
      Color = clBtnHighlight
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -12
      Font.Name = 'Arial'
      Font.Style = []
      ParentColor = False
      ParentFont = False
      OnClick = FildClick
      OnMouseDown = FildMouseDown
      OnMouseMove = FildMouseMove
      OnMouseUp = FildMouseUp
      OnPaint = FildPaint
    end
  end
  object Panel1: TPanel
    Left = 4
    Top = 430
    Width = 875
    Height = 80
    Alignment = taLeftJustify
    TabOrder = 1
    object Label1: TLabel
      Left = 11
      Top = 11
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
    object btnSave: TButton
      Left = 596
      Top = 45
      Width = 89
      Height = 25
      Caption = ' '#1042#1099#1093#1086#1076' '
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -13
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
      TabOrder = 0
      OnClick = btnSaveClick
    end
    object btnRef: TButton
      Left = 485
      Top = 44
      Width = 108
      Height = 25
      Caption = #1048#1085#1089#1090#1088#1091#1082#1094#1080#1103
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -16
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
      TabOrder = 1
      OnClick = btnRefClick
    end
    object SizeX: TEdit
      Left = 104
      Top = 8
      Width = 45
      Height = 24
      ReadOnly = True
      TabOrder = 2
      Text = '2'
    end
    object ButSizeX: TUpDown
      Left = 148
      Top = 8
      Width = 16
      Height = 25
      Min = -10000
      Max = 10000
      Position = 0
      TabOrder = 3
      Wrap = False
      OnClick = ButSizeXClick
    end
    object SizeY: TEdit
      Left = 256
      Top = 8
      Width = 45
      Height = 24
      ReadOnly = True
      TabOrder = 4
      Text = '300'
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
      TabOrder = 5
      Wrap = False
      OnClick = ButSizeYClick
    end
    object Button2: TButton
      Left = 315
      Top = 43
      Width = 170
      Height = 25
      Caption = ' '#1057#1084#1077#1085#1080#1090#1100' '#1089#1087#1086#1089#1086#1073' '#1087#1086#1082#1072#1079#1072
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -13
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
      TabOrder = 6
      OnClick = Button2Click
    end
    object ButCancel: TButton
      Left = 318
      Top = 9
      Width = 66
      Height = 25
      Caption = #1042' '#1085#1072#1095#1072#1083#1086
      TabOrder = 7
      OnClick = ButCancelClick
    end
    object Edit1: TEdit
      Left = 173
      Top = 43
      Width = 142
      Height = 24
      TabOrder = 8
    end
    object CheckBox1: TCheckBox
      Left = 20
      Top = 39
      Width = 153
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
      TabOrder = 9
      OnClick = CheckBox1Click
    end
    object Button1: TButton
      Left = 387
      Top = 9
      Width = 73
      Height = 25
      Caption = #1042' '#1082#1086#1085#1077#1094
      TabOrder = 10
      OnClick = Button1Click
    end
    object CheckBox2: TCheckBox
      Left = 581
      Top = 11
      Width = 168
      Height = 22
      Caption = #1064#1072#1075#1080' '#1074#1087#1088#1072#1074#1086'  '#1074' '#1082#1084
      Checked = True
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -16
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
      State = cbChecked
      TabOrder = 11
      OnClick = CheckBox2Click
    end
    object Edit2: TEdit
      Left = 750
      Top = 10
      Width = 34
      Height = 24
      TabOrder = 12
      Text = '1'
    end
    object UpDown1: TUpDown
      Left = 784
      Top = 10
      Width = 17
      Height = 24
      Associate = Edit2
      Min = 0
      Position = 1
      TabOrder = 13
      Thousands = False
      Wrap = True
      OnClick = UpDown1Click
    end
  end
  object Button3: TButton
    Left = 465
    Top = 438
    Width = 117
    Height = 26
    Caption = ' '#1057#1076#1074#1080#1075#1080' '#1087#1086' '#1096#1072#1075#1072#1084
    TabOrder = 2
    OnClick = Button3Click
  end
end
