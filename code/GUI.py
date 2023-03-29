import PySimpleGUI as sg
import database


class windows:
    def __init__(self, Colum_size: int = 18, Line_size: int = 10) -> None:
        self.block_column_size = Colum_size  # Tamanho das colunas
        self.block_line_size = Line_size  # Tamanho das linhas
        sg.change_look_and_feel('DarkGray13')

    def search(self, fields: dict):
        self.max_price = fields["price"]
        search_screen = self.create_search(fields)

        window = sg.Window(
            'Steam BUG',
            search_screen,
            element_justification='center',
            resizable=True
        )

        while True:
            event, values = window.read()
            if event == sg.WIN_CLOSED:
                break
            elif event == 'BUSCAR':
                values['-SEARCH_TYPE-'] = window['-SEARCH_TYPE-'].metadata
                for value in window.AllKeysDict:
                    if '-REV' in value:
                        values[value] = window[value].metadata
                self.searchDatabase(values)
            elif event == 'Limpar Filtros':
                self.cleanFields(window, fields)
            elif event == '-SEARCH_TYPE-':  # if the graphical button that changes images
                window['-SEARCH_TYPE-'].metadata = not window['-SEARCH_TYPE-'].metadata
                window['-SEARCH_TYPE-'].update(
                    image_data=self.toggle_btn_on if window['-SEARCH_TYPE-'].metadata else self.toggle_btn_off)
            elif '-REV' in event:
                window[event].metadata = not window[event].metadata
                window[event].update(
                    self.checked if window[event].metadata else self.unchecked)

    def fail_popup(self, title: str, text: str):
        """Cria a janela de erro"""
        sg.popup(title=title, text=text)

    def create_DB(self) -> str:
        """Cria a janela para procurar e criar a base de dados"""
        create_DB_screen = [
            [sg.Text('Caminho para DB')],
            [sg.Combo(sg.user_settings_get_entry('-filenames-', []), default_value=sg.user_settings_get_entry(
                '-last filename-', ''), size=(80, 1), key='-FILENAME-'), sg.FileBrowse()],
            [sg.Button('Go'), sg.Button('Exit')]
        ]

        event, values = sg.Window(
            'Insira o caminho para a sua DB', create_DB_screen).read(close=True)

        if event == 'Go':
            sg.user_settings_set_entry(
                '-filenames-', list(set(sg.user_settings_get_entry('-filenames-', []) + [values['-FILENAME-'], ])))
            sg.user_settings_set_entry('-last filename-', values['-FILENAME-'])

        return values['-FILENAME-']

    def translate_reviews(self, values: list[bool]):
        """Traduz os valores de reviews para o formato da base de dados"""
        reviews = []
        if values[0]:
            reviews.append('Overwhelmingly Positive')
        if values[1]:
            reviews.append('Very Positive')
        if values[2]:
            reviews.append('Positive')
        if values[3]:
            reviews.append('Mostly Positive')
        if values[4]:
            reviews.append('Mixed')
        if values[5]:
            reviews.append('Mostly Negative')
        if values[6]:
            reviews.append('Negative')
        if values[7]:
            reviews.append('Very Negative')
        if values[8]:
            reviews.append('Overwhelmingly Negative')
        return reviews

    def searchDatabase(self, values):
        """Procura na base de dados"""
        output: dict = {}
        output['name']: str = values['-NAME-']
        output['id']: int = int(values['-ID-']) if values['-ID-'] != '' else -1
        output['date']: list[str] = values['-DATA-']
        output['price_min']: int = - \
            1 if float(values['-PRICE_MIN-']
                       ) == 0 else int(float(values['-PRICE_MIN-']) * 100)
        output['price_max']: int = -1 if float(
            values['-PRICE_MAX-']) == self.max_price else int(float(values['-PRICE_MAX-']) * 100)
        output['genres']: list[str] = values['-GENERO-']
        output['languages']: list[str] = values['-IDIOMA-']
        output['tags']: list[str] = values['-TAGS-']
        output['developers']: list[str] = values['-DEV-']
        output['publishers']: list[str] = values['-PUBLISHER-']
        output['reviews']: list[str] = self.translate_reviews([values['-REV_EX_POS-'], values['-REV_MUITO_POS-'],
                                                               values['-REV_POS-'], values['-REV_LIG_POS-'],
                                                               values['-REV_NEUTRAS-'], values['-REV_LIG_NEG-'],
                                                               values['-REV_NEG-'], values['-REV_MUITO_NEG-'],
                                                               values['-REV_EX_NEG-']])
        output['decades']: int = - \
            1 if values['-DECADE-'] == '' else int(values['-DECADE-'])
        output['search']: bool = not values['-SEARCH_TYPE-']

        gamesFound = database.Search(output['genres'], output['languages'], output['tags'], output['date'], output['developers'], output['publishers'],
                                     output['reviews'], output['name'], output['price_min'], output['price_max'], output['decades'], output['id'], output['search'])
        fields = ["AppID", "Nome", "Desenvolvedor", "Publicador",
                  "Data de Lançamento", "Tags", "Preço", "Reviews"]

        self.showGames(gamesFound, fields)

    def showGames(self, gamesFound, fields: list[str]):
        """Mostra os jogos encontrados na janela de resultados"""
        # ------ Table Definition ------
        order: bool = False
        last_col: int = 0
        gamesFound.sort(key=lambda x: x[0], reverse=order)
        layout = [[sg.Table(values=gamesFound, headings=fields, max_col_width=20,
                            auto_size_columns=True,
                            # cols_justification=('left','center','right','c', 'l', 'bad'),       # Added on GitHub only as of June 2022
                            display_row_numbers=False,
                            right_click_selects=True,
                            justification='center',
                            num_rows=20,
                            alternating_row_color='black',
                            key='-TABLE-',
                            selected_row_colors='red on yellow',
                            enable_events=True,
                            expand_x=True,
                            expand_y=True,
                            vertical_scroll_only=False,
                            # Comment out to not enable header and other clicks
                            enable_click_events=True)]]

        # ------ Create Window ------
        window = sg.Window('Jogos Encontrados', layout,
                           # ttk_theme='clam',
                           # font='Helvetica 25',
                           resizable=True
                           )

        # ------ Event Loop ------
        while True:
            event, values = window.read()
            if event == sg.WIN_CLOSED:
                break
            if isinstance(event, tuple):
                if event[0] == '-TABLE-':
                    if event[2][0] == -1 and event[2][1] != -1:
                        col_clicked = event[2][1]
                        order = not order if col_clicked == last_col else False
                        last_col = col_clicked
                        gamesFound.sort(key=lambda x: x[col_clicked], reverse=order)
                        window['-TABLE-'].update(values=gamesFound)


        window.close()

    def cleanFields(self, window: sg.Window, fields: dict):
        def cleanButtonEvent(window: sg.Window, key: str, default: bool, btn_on: bytes, btn_off: bytes):
            window[key].metadata = default
            window[key].update(image_data=btn_on if window[key].metadata else btn_off)

        window['-ID-'].update('')
        window['-NAME-'].update('')
        window['-PRICE_MIN-'].update(0)
        window['-PRICE_MAX-'].update(fields["price"])
        window['-GENERO-'].update(values=fields["genres"])
        window['-IDIOMA-'].update(values=fields["languages"])
        window['-TAGS-'].update(values=fields["tags"])
        window['-DATA-'].update(values=fields["dates"])
        window['-DEV-'].update(values=fields["developers"])
        window['-PUBLISHER-'].update(values=fields["publishers"])
        window['-DECADE-'].update(values=fields["decades"])
        cleanButtonEvent(window, '-REV_EX_POS-', False, self.checked, self.unchecked)
        cleanButtonEvent(window, '-REV_MUITO_POS-', False, self.checked, self.unchecked)
        cleanButtonEvent(window, '-REV_POS-', False, self.checked, self.unchecked)
        cleanButtonEvent(window, '-REV_LIG_POS-', False, self.checked, self.unchecked)
        cleanButtonEvent(window, '-REV_NEUTRAS-', False, self.checked, self.unchecked)
        cleanButtonEvent(window, '-REV_LIG_NEG-', False, self.checked, self.unchecked)
        cleanButtonEvent(window, '-REV_NEG-', False, self.checked, self.unchecked)
        cleanButtonEvent(window, '-REV_MUITO_NEG-', False, self.checked, self.unchecked)
        cleanButtonEvent(window, '-REV_EX_NEG-', False, self.checked, self.unchecked)
        cleanButtonEvent(window, '-SEARCH_TYPE-', True, self.toggle_btn_on, self.toggle_btn_off)

    def create_search(self, fields: dict) -> list:
        """Layout da janela principal"""
        self.toggle_btn_off = b'iVBORw0KGgoAAAANSUhEUgAAACgAAAAoCAYAAACM/rhtAAAABmJLR0QA/wD/AP+gvaeTAAAED0lEQVRYCe1WTWwbRRR+M/vnv9hO7BjHpElMKSlpqBp6gRNHxAFVcKM3qgohQSqoqhQ45YAILUUVDRxAor2VAweohMSBG5ciodJUSVqa/iikaePEP4nj2Ovdnd1l3qqJksZGXscVPaylt7Oe/d6bb9/svO8BeD8vA14GvAx4GXiiM0DqsXv3xBcJU5IO+RXpLQvs5yzTijBmhurh3cyLorBGBVokQG9qVe0HgwiXLowdy9aKsY3g8PA5xYiQEUrsk93JTtjd1x3siIZBkSWQudUK4nZO1w3QuOWXV+HuP/fL85klAJuMCUX7zPj4MW1zvC0Ej4yMp/w++K2rM9b70sHBYCjo34x9bPelsgp/XJksZ7KFuwZjr3732YcL64ttEDw6cq5bVuCvgy/sje7rT0sI8PtkSHSEIRIKgCQKOAUGM6G4VoGlwiqoVd2Za9Vl8u87bGJqpqBqZOj86eEHGNch+M7otwHJNq4NDexJD+59RiCEQG8qzslFgN8ibpvZNsBifgXmFvJg459tiOYmOElzYvr2bbmkD509e1ylGEZk1Y+Ssfan18n1p7vgqVh9cuiDxJPxKPT3dfGXcN4Tp3dsg/27hUQs0qMGpRMYjLz38dcxS7Dm3nztlUAb38p0d4JnLozPGrbFfBFm79c8hA3H2AxcXSvDz7/+XtZE1kMN23hjV7LTRnKBh9/cZnAj94mOCOD32gi2EUw4FIRUMm6LGhyiik86nO5NBdGRpxYH14bbjYfJteN/OKR7UiFZVg5T27QHYu0RBxoONV9W8KQ7QVp0iXdE8fANUGZa0QAvfhhXlkQcmjJZbt631oIBnwKmacYoEJvwiuFgWncWnXAtuVBBEAoVVXWCaQZzxmYuut68b631KmoVBEHMUUrJjQLXRAQVSxUcmrKVHfjWWjC3XOT1FW5QrWpc5IJdQhDKVzOigEqS5dKHMVplnNOqrmsXqUSkn+YzWaHE9RW1FeXL7SKZXBFUrXW6jIV6YTEvMAUu0W/G3kcxPXP5ylQZs4fa6marcWvvZfJu36kuHjlc/nMSuXz+/ejxgqPFpuQ/xVude9eu39Jxu27OLvBGoMjrUN04zrNMbgVmOBZ96iPdPZmYntH5Ls76KuxL9NyoLA/brav7n382emDfHqeooXyhQmARVhSnAwNNMx5bu3V1+habun5nWdXhwJZ2C5mirTesyUR738sv7g88UQ0rEkTDlp+1wwe8Pf0klegUenYlgyg7bby75jUTITs2rhCAXXQ2vwxz84vlB0tZ0wL4NEcLX/04OrrltG1s8aOrHhk51SaK0us+n/K2xexBxljcsm1n6x/Fuv1PCWGiKOaoQCY1Vb9gWPov50+fdEqd21ge3suAlwEvA14G/ucM/AuppqNllLGPKwAAAABJRU5ErkJggg=='
        self.toggle_btn_on = b'iVBORw0KGgoAAAANSUhEUgAAACgAAAAoCAYAAACM/rhtAAAABmJLR0QA/wD/AP+gvaeTAAAD+UlEQVRYCe1XzW8bVRCffbvrtbP+2NhOD7GzLm1VoZaPhvwDnKBUKlVyqAQ3/gAkDlWgPeVQEUCtEOIP4AaHSI0CqBWCQyXOdQuRaEFOk3g3IMWO46+tvZ+PeZs6apq4ipON1MNafrvreTPzfvub92bGAOEnZCBkIGQgZOClZoDrh25y5pdjruleEiX+A+rCaQo05bpuvJ/+IHJCSJtwpAHA/e269g8W5RbuzF6o7OVjF8D3Pr4tSSkyjcqfptPDMDKSleW4DKIggIAD5Yf+Oo4DNg6jbUBlvWLUNutAwZu1GnDjzrcXzGcX2AHw/emFUV6Sfk0pqcKpEydkKSo9q3tkz91uF5aWlo1Gs/mYc+i7tz4//19vsW2AU9O381TiioVCQcnlRsWeQhD3bJyH1/MiFLICyBHiuzQsD1arDvypW7DR9nzZmq47q2W95prm+I9fXfqXCX2AF2d+GhI98Y8xVX0lnxvl2UQQg0csb78ag3NjEeD8lXZ7pRTgftmCu4864OGzrq+5ZU0rCa3m+NzXlzvoAoB3+M+SyWQuaHBTEzKMq/3BMbgM+FuFCDBd9kK5XI5PJBKqLSev+POTV29lKB8rT0yMD0WjUSYLZLxzNgZvIHODOHuATP72Vwc6nQ4Uiw8MUeBU4nHS5HA6TYMEl02wPRcZBJuv+ya+UCZOIBaLwfCwQi1Mc4QXhA+PjWRkXyOgC1uIhW5Qd8yG2TK7kSweLcRGKKVnMNExWWBDTQsH9qVmtmzjiThQDs4Qz/OUSGTwcLwIQTLW58i+yOjpXDLqn1tgmDzXzRCk9eDenjo9yhvBmlizrB3V5dDrNTuY0A7opdndStqmaQLPC1WCGfShYRgHdLe32UrV3ntiH9LliuNrsToNlD4kruN8v75eafnSgC6Luo2+B3fGKskilj5muV6pNhk2Qqg5v7lZ51nBZhNBjGrbxfI1+La5t2JCzfD8RF1HTBGJXyDzs1MblONulEqPDVYXgwDIfNx91IUVbAbY837GMur+/k/XZ75UWmJ77ou5mfM1/0x7vP1ls9XQdF2z9uNsPzosXPNFA5m0/EX72TBSiqsWzN8z/GZB08pWq9VeEZ+0bjKb7RTD2i1P4u6r+bwypo5tZUumEcDAmuC3W8ezIqSGfE6g/sTd1W5p5bKjaWubrmWd29Fu9TD0GlYlmTx+8tTJoZeqYe2BZC1/JEU+wQR5TVEUPptJy3Fs+Vkzgf8lemqHumP1AnYoMZSwsVEz6o26i/G9Lgitb+ZmLu/YZtshfn5FZDPBCcJFQRQ+8ih9DctOFvdLIKHH6uUQnq9yhFu0bec7znZ+xpAGmuqef5/wd8hAyEDIQMjAETHwP7nQl2WnYk4yAAAAAElFTkSuQmCC'
        self.checked = b'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'
        self.unchecked = b'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'
        return [[sg.Text('STEAM BUG'),],
                [
                sg.Text('GENERO', justification="center",
                        size=self.block_column_size-3, expand_x=True),
                sg.Text('IDIOMA', justification="center",
                        size=self.block_column_size-3, expand_x=True),
                sg.Text('TAGS', justification="center",
                        size=self.block_column_size-3, expand_x=True),
                sg.Text('DATA', justification="center",
                        size=self.block_column_size-3, expand_x=True),
                sg.Text('DEV', justification="center",
                        size=self.block_column_size-3, expand_x=True),
                sg.Text('PUBLISHER', justification="center",
                        size=self.block_column_size-3, expand_x=True)
                ],
                [
                sg.Listbox(values=fields["genres"], select_mode=sg.SELECT_MODE_MULTIPLE, size=(
                    self.block_column_size - 3, self.block_line_size), key='-GENERO-', expand_x=True, expand_y=True),
                sg.Listbox(values=fields["languages"], select_mode=sg.SELECT_MODE_MULTIPLE, size=(
                    self.block_column_size - 3, self.block_line_size), key='-IDIOMA-', expand_x=True, expand_y=True),
                sg.Listbox(values=fields["tags"], select_mode=sg.SELECT_MODE_MULTIPLE, size=(
                    self.block_column_size - 3, self.block_line_size), key='-TAGS-', expand_x=True, expand_y=True),
                sg.Listbox(values=fields["dates"], select_mode=sg.SELECT_MODE_MULTIPLE, size=(
                    self.block_column_size - 3, self.block_line_size), key='-DATA-', expand_x=True, expand_y=True),
                sg.Listbox(values=fields["developers"], select_mode=sg.SELECT_MODE_MULTIPLE, size=(
                    self.block_column_size - 3, self.block_line_size), key='-DEV-', expand_x=True, expand_y=True),
                sg.Listbox(values=fields["publishers"], select_mode=sg.SELECT_MODE_MULTIPLE, size=(
                    self.block_column_size - 3, self.block_line_size), key='-PUBLISHER-', expand_x=True, expand_y=True)
                ],
                [
                sg.Push(), sg.Text('Preço MIN:', justification="center", expand_x=True), sg.Push(), sg.Text(
                    'Preço MAX:', justification="center", expand_x=True), sg.Push()
                ],
                [
                sg.Push(), sg.Slider(orientation="h", range=(0, self.max_price), resolution=0.01, key='-PRICE_MIN-', expand_x=True), sg.Slider(
                    orientation="h", range=(0, self.max_price), resolution=0.01, default_value=fields["price"], key='-PRICE_MAX-', expand_x=True), sg.Push()
                ],
                [
                sg.Text('Década:'), sg.Combo(
                    values=fields["decades"], key='-DECADE-')
                ],
                [
                [sg.Text('Review:')],
                [
                    sg.Image(self.unchecked, key='-REV_EX_POS-', metadata=False,
                             enable_events=True), sg.Text('Extremamente Positivas', size=self.block_column_size),
                    sg.Image(self.unchecked, key='-REV_MUITO_POS-', metadata=False,
                              enable_events=True), sg.Text('Muito Positivas', size=self.block_column_size),
                    sg.Image(self.unchecked, key='-REV_POS-', metadata=False,
                             enable_events=True), sg.Text('Positivas', size=self.block_column_size),
                ],
                [
                    sg.Image(self.unchecked, key='-REV_LIG_POS-', metadata=False,
                             enable_events=True), sg.Text('Ligeiramente Positivas', size=self.block_column_size),
                    sg.Image(self.unchecked, key='-REV_NEUTRAS-', metadata=False,
                             enable_events=True), sg.Text('Neutras', size=self.block_column_size),
                    sg.Image(self.unchecked, key='-REV_LIG_NEG-', metadata=False,
                             enable_events=True), sg.Text('Ligeiramente Negativas', size=self.block_column_size),
                ],
                [
                    sg.Image(self.unchecked, key='-REV_NEG-', metadata=False,
                             enable_events=True), sg.Text('Negativas', size=self.block_column_size),
                    sg.Image(self.unchecked, key='-REV_MUITO_NEG-', metadata=False,
                             enable_events=True), sg.Text('Muito Negativas', size=self.block_column_size),
                    sg.Image(self.unchecked, key='-REV_EX_NEG-', metadata=False,
                             enable_events=True), sg.Text('Extremamente Negativas', size=self.block_column_size),
                ]
                ],
                [
                sg.Text('AppID:'), sg.InputText(
                    key='-ID-', size=10),
                sg.Text('Nome:'), sg.InputText(
                    key='-NAME-')
                ],
                [
                sg.Button('BUSCAR'),
                sg.Button('Limpar Filtros'),
                [sg.Text('Tipo de Busca:')],
                [sg.Text('OR'),
                    sg.Button(image_data=self.toggle_btn_on, key='-SEARCH_TYPE-', button_color=(
                        sg.theme_background_color(), sg.theme_background_color()), border_width=0, metadata=True),
                    sg.Text('AND')],
                sg.Button('ADICIONAR')
                ]
                ]


if __name__ == "__main__":
    print("Programa de GUI não pode ser executado diretamente!")
