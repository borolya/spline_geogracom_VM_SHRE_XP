object Form5: TForm5
  Left = 272
  Top = 127
  Width = 550
  Height = 469
  Hint = #1059#1089#1090#1072#1085#1086#1074#1080#1090#1100' '#1090#1088#1077#1073#1091#1077#1084#1099#1077' '#1079#1085#1072#1095#1077#1085#1080#1103' '#1085#1086#1088#1084#1072#1090#1080#1074#1086#1074
  Caption = #1056#1077#1076#1072#1082#1090#1080#1088#1086#1074#1072#1085#1080#1077' '#1085#1086#1088#1084#1072#1090#1080#1074#1086#1074
  Color = clGradientActiveCaption
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -19
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  ShowHint = True
  OnCreate = FormCreate
  PixelsPerInch = 96
  TextHeight = 24
  object Label1: TLabel
    Left = 10
    Top = 40
    Width = 376
    Height = 24
    Caption = #1057#1082#1086#1088#1086#1089#1090#1100' '#1087#1072#1089#1089#1072#1078#1080#1088#1089#1082#1086#1075#1086' '#1087#1086#1077#1079#1076#1072' '#1074' '#1082#1084'/'#1095#1072#1089
  end
  object Label2: TLabel
    Left = 12
    Top = 72
    Width = 331
    Height = 24
    Caption = #1057#1082#1086#1088#1086#1089#1090#1100' '#1075#1088#1091#1079#1086#1074#1086#1075#1086' '#1087#1086#1077#1079#1076#1072' '#1074' '#1082#1084'/'#1095#1072#1089
  end
  object Label3: TLabel
    Left = 14
    Top = 104
    Width = 281
    Height = 16
    Caption = #1053#1072#1095#1072#1083#1100#1085#1086#1077' '#1074#1086#1079#1074#1099#1096#1077#1085#1080#1077' h0 '#1074' '#1084#1084' '#1074' '#1092#1086#1088#1084#1091#1083#1077
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -13
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
  end
  object Label4: TLabel
    Left = 15
    Top = 136
    Width = 351
    Height = 24
    Caption = #1052#1072#1082#1089#1080#1084#1072#1083#1100#1085#1086#1077' '#1074#1086#1079#1074#1099#1096#1077#1085#1080#1077' hmax '#1074' '#1084#1084
  end
  object Label5: TLabel
    Left = 14
    Top = 168
    Width = 509
    Height = 24
    Caption = #1053#1077#1087#1086#1075#1072#1096#1077#1085#1085#1086#1077' '#1091#1089#1082#1086#1088#1077#1085#1080#1077' '#1075#1088#1091#1079#1086#1074#1086#1075#1086' '#1087#1086#1077#1079#1076#1072' '#1074' '#1084'/'#1089#1077#1082'/'#1089#1077#1082':'
  end
  object Label6: TLabel
    Left = 299
    Top = 200
    Width = 59
    Height = 24
    Caption = '   '#1040'min'
  end
  object Label7: TLabel
    Left = 16
    Top = 269
    Width = 378
    Height = 24
    Caption = 'k=1/'#1091#1082#1083#1086#1085' '#1086#1090#1074#1086#1076#1072'  '#1074#1086#1079#1074#1099#1096#1077#1085#1080#1103' ; (lmin=k*h)'
  end
  object Label8: TLabel
    Left = 16
    Top = 121
    Width = 99
    Height = 16
    Caption = 'h=12.5V*V/R +h0'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -13
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
  end
  object Label9: TLabel
    Left = 302
    Top = 232
    Width = 59
    Height = 24
    Caption = '  Amax'
  end
  object Edit1: TEdit
    Left = 390
    Top = 40
    Width = 65
    Height = 32
    TabOrder = 0
    Text = '120'
  end
  object Edit2: TEdit
    Left = 390
    Top = 72
    Width = 65
    Height = 32
    TabOrder = 1
    Text = '80'
  end
  object Edit3: TEdit
    Left = 389
    Top = 104
    Width = 65
    Height = 32
    TabOrder = 2
    Text = ' -114'
  end
  object Edit4: TEdit
    Left = 388
    Top = 136
    Width = 65
    Height = 32
    TabOrder = 3
    Text = '153'
  end
  object Edit5: TEdit
    Left = 391
    Top = 193
    Width = 66
    Height = 32
    TabOrder = 4
    Text = '-0.40'
  end
  object Edit6: TEdit
    Left = 393
    Top = 227
    Width = 65
    Height = 32
    TabOrder = 5
    Text = '0.35'
  end
  object Edit7: TEdit
    Left = 396
    Top = 268
    Width = 60
    Height = 32
    TabOrder = 6
    Text = '2.0'
  end
  object Button1: TButton
    Left = 40
    Top = 314
    Width = 169
    Height = 33
    Caption = #1053#1072#1095#1072#1090#1100' '#1087#1088#1086#1074#1077#1088#1082#1091
    TabOrder = 7
    OnClick = Button1Click
  end
  object Button2: TButton
    Left = 296
    Top = 315
    Width = 161
    Height = 33
    Caption = #1042#1086#1079#1074#1088#1072#1090
    TabOrder = 8
    OnClick = Button2Click
  end
end
