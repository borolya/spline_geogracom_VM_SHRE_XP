object Form1: TForm1
  Left = 29
  Top = 121
  Width = 980
  Height = 583
  Caption = '  '#1059#1075#1083#1086#1075#1088#1072#1084#1084#1072
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  OnClose = FormClose
  OnResize = FormResize
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object TPaintBox
    Left = 376
    Top = 136
    Width = 105
    Height = 105
  end
  object MainBox: TScrollBox
    Left = 0
    Top = 0
    Width = 964
    Height = 450
    Align = alTop
    Anchors = [akTop, akRight, akBottom]
    TabOrder = 0
    object Fild: TPaintBox
      Left = 0
      Top = 0
      Width = 921
      Height = 436
      Color = clBtnHighlight
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'Arial'
      Font.Style = []
      ParentColor = False
      ParentFont = False
      PopupMenu = PopupMenu1
      OnDblClick = FildDblClick
      OnMouseDown = FildMouseDown
      OnMouseMove = FildMouseMove
      OnMouseUp = FildMouseUp
      OnPaint = FildPaint
    end
  end
  object Panel1: TPanel
    Left = -8
    Top = 438
    Width = 969
    Height = 99
    Alignment = taLeftJustify
    TabOrder = 1
    object Label1: TLabel
      Left = 16
      Top = 8
      Width = 71
      Height = 13
      Caption = #1052#1072#1089#1096#1090#1072#1073' '#1087#1086' '#1061
    end
    object Label4: TLabel
      Left = 168
      Top = 8
      Width = 71
      Height = 13
      Caption = #1052#1072#1089#1096#1090#1072#1073' '#1087#1086' Y'
    end
    object ButCenter: TSpeedButton
      Left = 528
      Top = 8
      Width = 57
      Height = 25
      AllowAllUp = True
      GroupIndex = 1
      Down = True
      Caption = #1057#1076#1074#1080#1075
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -13
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
      OnClick = ButCenterClick
    end
    object btnDisactive: TButton
      Left = 22
      Top = 41
      Width = 150
      Height = 25
      Caption = #1054#1090#1084#1077#1085#1080#1090#1100' '#1074#1099#1076#1077#1083#1077#1085#1080#1077
      Enabled = False
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -13
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
      TabOrder = 0
      OnClick = btnDisactiveClick
    end
    object btnMove: TButton
      Left = 175
      Top = 43
      Width = 105
      Height = 25
      Caption = #1055#1077#1088#1077#1084#1077#1089#1090#1080#1090#1100
      Enabled = False
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -13
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
      TabOrder = 1
      OnClick = btnMoveClick
    end
    object btnDelete: TButton
      Left = 287
      Top = 44
      Width = 110
      Height = 25
      Caption = #1059#1076#1072#1083#1080#1090#1100
      Enabled = False
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -13
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
      TabOrder = 2
      OnClick = btnDeleteClick
    end
    object btnSave: TButton
      Left = 534
      Top = 46
      Width = 94
      Height = 25
      Caption = #1057#1086#1093#1088#1072#1085#1080#1090#1100
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -13
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
      TabOrder = 3
      OnClick = btnSaveClick
    end
    object btnRef: TButton
      Left = 416
      Top = 45
      Width = 101
      Height = 25
      Caption = #1057#1087#1088#1072#1074#1082#1072
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -13
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
      TabOrder = 4
      OnClick = btnRefClick
    end
    object SizeX: TEdit
      Left = 88
      Top = 8
      Width = 49
      Height = 21
      ReadOnly = True
      TabOrder = 5
      Text = '0'
    end
    object ButSizeX: TUpDown
      Left = 136
      Top = 8
      Width = 17
      Height = 25
      Min = -10000
      Max = 10000
      Position = 0
      TabOrder = 6
      Wrap = False
      OnClick = ButSizeXClick
    end
    object SizeY: TEdit
      Left = 240
      Top = 8
      Width = 57
      Height = 21
      ReadOnly = True
      TabOrder = 7
      Text = '0'
    end
    object ButSizeY: TUpDown
      Left = 296
      Top = 8
      Width = 17
      Height = 25
      Min = -10000
      Max = 10000
      Increment = 10
      Position = 0
      TabOrder = 8
      Wrap = False
      OnClick = ButSizeYClick
    end
    object isShow: TCheckBox
      Left = 328
      Top = 16
      Width = 193
      Height = 17
      Caption = #1055#1086#1082#1072#1079#1072#1090#1100' '#1087#1088#1086#1077#1082#1090#1085#1091#1102' '#1091#1075#1083#1086#1075#1088#1072#1084#1084#1091
      TabOrder = 9
      OnClick = isShowClick
    end
    object Button1: TButton
      Left = 638
      Top = 45
      Width = 75
      Height = 25
      Caption = #1042#1086#1079#1074#1088#1072#1090
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -13
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
      TabOrder = 10
      OnClick = Button1Click
    end
    object Button2: TButton
      Left = 718
      Top = 45
      Width = 139
      Height = 25
      Caption = #1047#1072#1082#1086#1085#1095#1080#1090#1100' '#1088#1072#1073#1086#1090#1091' '
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -13
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
      TabOrder = 11
      OnClick = Button2Click
    end
    object ButCancel: TButton
      Left = 601
      Top = 9
      Width = 70
      Height = 25
      Caption = #1042' '#1085#1072#1095#1072#1083#1086
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -13
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
      TabOrder = 12
      OnClick = ButCancelClick
    end
    object Button3: TButton
      Left = 683
      Top = 10
      Width = 75
      Height = 23
      Caption = '  '#1042' '#1082#1086#1085#1077#1094
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -13
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
      TabOrder = 13
      OnClick = Button3Click
    end
  end
  object PopupMenu1: TPopupMenu
    Left = 18
    Top = 9
    object N4: TMenuItem
      Caption = #1054#1090#1084#1077#1085#1080#1090#1100' '#1074#1099#1076#1077#1083#1077#1085#1080#1077
      Enabled = False
      OnClick = N2Click
    end
    object N3: TMenuItem
      Caption = #1055#1077#1088#1077#1084#1077#1089#1090#1080#1090#1100
      Enabled = False
      OnClick = N3Click
    end
    object N2: TMenuItem
      Caption = #1059#1076#1072#1083#1080#1090#1100
      Enabled = False
      OnClick = N4Click
    end
  end
end
