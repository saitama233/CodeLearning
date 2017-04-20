from win32com import client as wc

class HandleWord:
    def __init__(self, filename):
        self.w = wc.Dispatch("Word.Application")
        self.w.Visible = 1
        self.w.DisplayAlerts = 1
        # self.w.Document.add()
        # self.openword()

    def openword(self):
        doc = self.w.Document.add()

    def writetext(self, text):
        pass

    def writeimg(self, filename):
        pass
