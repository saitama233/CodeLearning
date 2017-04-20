#!/usr/bin/env python
# -*- encoding:utf-8 -*-

import wx


class MyApp(wx.App):
    def OnInit(self):
        frame = MyFrame(parent=None, id=-1, title='ExampleBoxSizer')
        frame.Show(True)
        return True


class MyFrame(wx.Frame):
    def __init__(self, parent, id, title):
        wx.Frame.__init__(self, parent, id, title, size=(778, 494),
                          style=wx.DEFAULT_FRAME_STYLE | wx.RESIZE_BORDER | wx.MAXIMIZE_BOX)
        self.panel = wx.Panel(self, -1)

        h_box_sizer = wx.BoxSizer(wx.HORIZONTAL)

        self.file_path = wx.TextCtrl(self.panel, -1)
        self.open_button = wx.Button(self.panel, -1, label=u'打开')
        self.save_button = wx.Button(self.panel, -1, label=u'保存')

        h_box_sizer.Add(self.file_path, proportion=1, flag=wx.EXPAND | wx.ALL, border=5)
        h_box_sizer.Add(self.open_button, proportion=0, flag=wx.ALL, border=5)
        h_box_sizer.Add(self.save_button, proportion=0, flag=wx.ALL, border=5)

        self.edit_text = wx.TextCtrl(self.panel, style=wx.TE_MULTILINE | wx.TE_RICH2 | wx.HSCROLL)

        v_box_sizer = wx.BoxSizer(wx.VERTICAL)
        v_box_sizer.Add(h_box_sizer, proportion=0, flag=wx.EXPAND)
        v_box_sizer.Add(self.edit_text, proportion=1, flag=wx.EXPAND, border=5)

        self.panel.SetSizer(v_box_sizer)


def main():
    app = MyApp()
    app.MainLoop()


if __name__ == '__main__':
    main()  