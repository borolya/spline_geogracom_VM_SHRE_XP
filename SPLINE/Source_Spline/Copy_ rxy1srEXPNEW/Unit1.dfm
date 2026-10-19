object Form1: TForm1
  Left = -3
  Top = 134
  Width = 872
  Height = 383
  Anchors = []
  Caption = #1056#1072#1089#1095#1105#1090' '#1087#1086' '#1082#1086#1086#1088#1076#1080#1085#1072#1090#1072#1084'  '#1057#1090#1088#1091#1095#1077#1085#1082#1086#1074' '#1042'.'#1048'.'
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
    Left = 189
    Top = 5
    Width = 540
    Height = 25
    Color = clSkyBlue
    TabOrder = 0
    object Label1: TLabel
      Left = 1
      Top = 1
      Width = 179
      Height = 23
      Align = alLeft
      AutoSize = False
      Caption = '               '#1054#1073#1098#1077#1082#1090'     '
      Color = clRed
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -16
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentColor = False
      ParentFont = False
    end
    object Label2: TLabel
      Left = 180
      Top = 1
      Width = 206
      Height = 23
      Align = alClient
      Anchors = [akLeft, akTop, akBottom]
      AutoSize = False
      Caption = '              '#1059#1095#1072#1089#1090#1086#1082
      Color = clLime
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -16
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentColor = False
      ParentFont = False
    end
    object Label3: TLabel
      Left = 386
      Top = 1
      Width = 153
      Height = 23
      Align = alRight
      AutoSize = False
      Caption = '          '#1042#1072#1088#1080#1072#1085#1090
      Color = clFuchsia
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -16
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentColor = False
      ParentFont = False
    end
  end
  object ras: TPanel
    Left = 0
    Top = 95
    Width = 185
    Height = 34
    BorderWidth = 1
    BorderStyle = bsSingle
    Caption = ' '#1056#1072#1089#1095#1105#1090' '#1091#1095#1072#1089#1090#1082#1072
    Color = clSkyBlue
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -16
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
    TabOrder = 1
    Visible = False
    OnClick = rasClick
  end
  object cop: TPanel
    Left = 0
    Top = 127
    Width = 185
    Height = 38
    BorderWidth = 1
    BorderStyle = bsSingle
    Caption = ' '#1050#1086#1087#1080#1088#1086#1074#1072#1085#1080#1077' '#1091#1095#1072#1089#1090#1082#1072
    Color = clSkyBlue
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -16
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
    TabOrder = 2
    Visible = False
    OnClick = copClick
  end
  object udal: TPanel
    Left = 0
    Top = 162
    Width = 185
    Height = 39
    BorderWidth = 1
    BorderStyle = bsSingle
    Caption = #1059#1076#1072#1083#1077#1085#1080#1077' '#1091#1095#1072#1089#1090#1082#1072
    Color = clSkyBlue
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -16
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
    TabOrder = 3
    Visible = False
    OnClick = udalClick
  end
  object fin: TPanel
    Left = 0
    Top = 199
    Width = 185
    Height = 42
    BorderWidth = 1
    BorderStyle = bsSingle
    Caption = #1047#1072#1082#1086#1085#1095#1080#1090#1100' '#1088#1072#1073#1086#1090#1091
    Color = clSkyBlue
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -16
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
    TabOrder = 4
    OnClick = finClick
  end
  object Gr2: TGroupBox
    Left = -2
    Top = 248
    Width = 192
    Height = 209
    Color = clSkyBlue
    ParentColor = False
    TabOrder = 6
    TabStop = True
    Visible = False
    object Button1: TButton
      Left = 3
      Top = 8
      Width = 185
      Height = 25
      Caption = #1042#1099#1073#1086#1088' '#1074#1072#1088#1080#1072#1085#1090#1072
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -16
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
      TabOrder = 0
      TabStop = False
      OnClick = Button1Click
    end
    object Button2: TButton
      Left = 3
      Top = 32
      Width = 185
      Height = 25
      Caption = #1050#1086#1087#1080#1088#1086#1074#1072#1085#1080#1077' '#1074#1072#1088#1080#1072#1085#1090#1072
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -16
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
      TabOrder = 1
      Visible = False
      OnClick = Button2Click
    end
    object Button3: TButton
      Left = 4
      Top = 56
      Width = 184
      Height = 25
      Caption = #1059#1076#1072#1083#1077#1085#1080#1077' '#1074#1072#1088#1080#1072#1085#1090#1072
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -16
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
      TabOrder = 2
      Visible = False
      OnClick = Button3Click
    end
    object Button4: TButton
      Left = 4
      Top = 80
      Width = 184
      Height = 25
      Caption = #1059#1075#1083#1086#1075#1088#1072#1084#1084#1072
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -16
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
      TabOrder = 3
      Visible = False
      OnClick = Button4Click
    end
    object Button5: TButton
      Left = 3
      Top = 104
      Width = 185
      Height = 25
      Caption = #1054#1087#1090#1080#1084#1080#1079#1072#1094#1080#1103
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -16
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
      TabOrder = 4
      Visible = False
      OnClick = Button5Click
    end
    object Button6: TButton
      Left = 4
      Top = 128
      Width = 183
      Height = 25
      Caption = #1055#1077#1095#1072#1090#1100
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -16
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
      TabOrder = 5
      Visible = False
      OnClick = Button6Click
    end
    object Button7: TButton
      Left = 4
      Top = 152
      Width = 183
      Height = 25
      Caption = #1057#1082#1086#1088#1086#1089#1090#1080' '#1080' '#1074#1086#1079#1074#1099#1096#1077#1085#1080#1103
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -15
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
      TabOrder = 6
      Visible = False
      OnClick = Button7Click
    end
    object Button8: TButton
      Left = 4
      Top = 184
      Width = 184
      Height = 25
      Caption = #1042#1086#1079#1074#1088#1072#1090
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -16
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
      TabOrder = 7
      OnClick = Button8Click
    end
  end
  object vibor: TPanel
    Left = 0
    Top = 61
    Width = 185
    Height = 35
    BorderWidth = 1
    BorderStyle = bsSingle
    Caption = #1042#1099#1073#1086#1088' '#1091#1095#1072#1089#1090#1082#1072
    Color = clSkyBlue
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -16
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
    TabOrder = 5
    OnClick = viborClick
  end
  object sgTrack: TStringGrid
    Left = 233
    Top = 72
    Width = 608
    Height = 369
    DefaultDrawing = False
    FixedCols = 0
    RowCount = 3
    Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine, goRowSizing, goEditing, goTabs, goAlwaysShowEditor]
    TabOrder = 7
    Visible = False
    OnClick = sgTrackClick
    OnDrawCell = sgTrackDrawCell
    ColWidths = (
      54
      94
      142
      155
      112)
    RowHeights = (
      24
      24
      24)
  end
  object Panel2: TPanel
    Left = 189
    Top = 28
    Width = 540
    Height = 30
    Color = clSkyBlue
    TabOrder = 8
    object peregon: TLabel
      Left = 1
      Top = 1
      Width = 185
      Height = 28
      Align = alLeft
      AutoSize = False
      Color = clWhite
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -16
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentColor = False
      ParentFont = False
    end
    object var: TLabel
      Left = 385
      Top = 1
      Width = 154
      Height = 28
      Align = alRight
      AutoSize = False
      Color = clWhite
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -16
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentColor = False
      ParentFont = False
      OnClick = varClick
    end
    object uch: TLabel
      Left = 186
      Top = 1
      Width = 199
      Height = 28
      Align = alClient
      AutoSize = False
      Color = clWhite
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -16
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentColor = False
      ParentFont = False
    end
  end
  object RG: TRadioGroup
    Left = 376
    Top = 192
    Width = 361
    Height = 177
    Caption = '                '#1053#1072#1095#1072#1083#1100#1085#1072#1103' '#1090#1086#1095#1082#1072' '#1080' '#1072#1079#1080#1084#1091#1090
    Color = clLime
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -16
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    Items.Strings = (
      #1047#1072#1076#1072#1102#1090#1089#1103' '#1074#1088#1091#1095#1085#1091#1102
      #1054#1087#1088#1077#1076#1077#1083#1103#1102#1090#1089#1103' '#1076#1074#1091#1084#1103' '#1087#1077#1088#1074#1099#1084#1080' '#1090#1086#1095#1082#1072#1084#1080
      #1054#1087#1088#1077#1076#1077#1083#1103#1102#1090#1089#1103' '#1090#1088#1077#1084#1103' '#1087#1077#1088#1074#1099#1084#1080' '#1090#1086#1095#1082#1072#1084#1080)
    ParentColor = False
    ParentFont = False
    TabOrder = 9
    Visible = False
    OnClick = RGClick
  end
end
