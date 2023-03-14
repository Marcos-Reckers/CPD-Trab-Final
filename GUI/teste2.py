import PySimpleGUI as sg


def setPlaceholder(widget, placeholderText):
    if getattr(widget, "WxTextCtrl", None):
        textCtrl = widget.WxTextCtrl
        textCtrl.SetHint(placeholderText)
        return

    if getattr(widget, "QT_QLineEdit", None): # SimpleGUIQt
        lineEdit = widget.QT_QLineEdit # type: QWidgets.QLineEdit
        lineEdit.setPlaceholderText(placeholderText)
        return

    if getattr(widget, "Widget", None) and hasattr(widget.Widget, "attributes"): # SimpleGUIWeb
        textInput = widget.Widget # type: remi.gui.TextInput
        textInput.attributes["placeholder"] = placeholderText
        return

    if getattr(widget, "TKEntry", None):
        # NOTE: tk8.7 will support "-placeholder" option
        #
        #   entry = widget.TKEntry
        #   entry.config(placeholder=placeholderText) # XXX: 未検証
        #   return entry.get
        #
        entry = widget.TKEntry

        def resetCursor(event=None):
            if entry.get() == placeholderText:
                entry.after_idle(entry.icursor, 0)

        def startInput(event=None):
            if entry.get() == placeholderText:
                entry.delete(0, "end")
                entry.config(fg="black")
            else:
                entry.after_idle(showPlaceholder)

        def showPlaceholder(event=None):
            if entry.get() == placeholderText or not entry.get():
                entry.delete(0, "end")
                entry.insert(0, placeholderText)
                entry.config(fg="gray50")
                entry.after_idle(entry.icursor, 0)

        entry.bind("<FocusIn>", resetCursor)
        entry.bind("<FocusOut>", showPlaceholder)
        entry.bind("<Button-1>", resetCursor)
        entry.bind("<Key>", startInput)
        showPlaceholder()

        def get_value():
            # ※ Tk の場合のみ
            # 空欄の場合、placeholder のテキストを入力値と勘違いしてしまうので、
            # placeholder を表示中の場合は、空文字を返す、この関数を通じて実際の値を所得します。
            # 
            # tk8.7 で placeholder オプションが実装されれば、廃止。
            # 
            text = entry.get()
            return text if text != placeholderText else ""

        widget.Get = get_value
        return

    import warnings
    warnings.warn("Unknown GUI Platform, setPlaceholder was ignored.")


layout = [
    [sg.Input("", key="load")],
    [sg.Button("OK")]
]

window = sg.Window("Placeholder", layout).Finalize()

# NOTE: Finalize() を呼んで、ウィジェットの生成完了を待つ
setPlaceholder(window["load"], placeholderText="FILE:")

while True:
    event, values = window.read()

    if event == sg.WIN_CLOSED:
        break
    
    if event == "OK":
        print("value: {}".format(values["load"]))

window.close()