object MainForm: TMainForm
  Left = 334
  Top = 80
  Width = 538
  Height = 360
  BorderIcons = [biSystemMenu, biMinimize]
  Caption = #1055#1083#1072#1085'.'#1042#1074#1086#1076' '#1076#1072#1085#1085#1099#1093'  '#1086#1073#1098#1077#1082#1090'  '
  Color = clMenuBar
  Ctl3D = False
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -16
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  OnCreate = FormCreate
  DesignSize = (
    522
    321)
  PixelsPerInch = 96
  TextHeight = 20
  object GroupBox1: TGroupBox
    Left = 8
    Top = 13
    Width = 491
    Height = 225
    Anchors = [akLeft, akTop, akRight]
    Ctl3D = True
    ParentCtl3D = False
    TabOrder = 0
    DesignSize = (
      491
      225)
    object Button1: TButton
      Left = -13
      Top = 3
      Width = 286
      Height = 33
      Caption = #1042#1074#1086#1076' '#1050#1088#1080#1074#1099#1093
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -17
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
      TabOrder = 0
      OnClick = Button1Click
    end
    object Button2: TButton
      Left = -93
      Top = 35
      Width = 367
      Height = 33
      Anchors = [akLeft, akTop, akRight]
      Caption = '                            '#1042#1074#1086#1076' '#1059#1095#1072#1089#1090#1082#1072
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -17
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
      TabOrder = 1
      OnClick = Button2Click
    end
    object Button4: TButton
      Left = -91
      Top = 67
      Width = 365
      Height = 33
      Anchors = [akLeft, akTop, akRight]
      Caption = '                             '#1059#1076#1072#1083#1077#1085#1080#1077' '#1050#1088#1080#1074#1099#1093
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -17
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
      TabOrder = 2
      OnClick = Button4Click
    end
    object Button3: TButton
      Left = -67
      Top = 98
      Width = 341
      Height = 34
      Anchors = [akLeft, akTop, akRight]
      Caption = '                            '#1059#1076#1072#1083#1077#1085#1080#1077' '#1059#1095#1072#1089#1090#1082#1072
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -17
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
      TabOrder = 3
      OnClick = Button3Click
    end
    object Button5: TButton
      Left = 0
      Top = 131
      Width = 274
      Height = 32
      Caption = '       '#1042#1086#1079#1074#1088#1072#1090
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -17
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
      TabOrder = 4
      OnClick = Button5Click
    end
  end
end
