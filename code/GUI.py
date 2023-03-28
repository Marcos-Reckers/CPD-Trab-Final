import PySimpleGUI as sg
import database

class windows:
    def __init__(self, Colum_size: int = 18, Line_size: int = 10) -> None:
        self.block_column_size = Colum_size  # Tamanho das colunas
        self.block_line_size = Line_size  # Tamanho das linhas

    def search(self, fields: dict):
        self.max_price = fields["price"]
        search_screen = self.create_search(fields)

        window = sg.Window(
            'Steam BUG',
            search_screen,
            element_justification='center'
        )

        while True:
            event, values = window.read()
            if event == sg.WIN_CLOSED:
                break
            elif event == 'BUSCAR':
                self.searchDatabase(values)
            elif event == 'Limpar Filtros':
                self.cleanFields(window, fields)

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
        output['price_min']: int = -1 if float(values['-PRICE_MIN-']) == 0 else int(float(values['-PRICE_MIN-']) * 100)
        output['price_max']: int = -1 if float(values['-PRICE_MAX-']) == self.max_price else int(float(values['-PRICE_MAX-']) * 100)
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
        output['decades']: int = -1 if values['-DECADE-'] == '' else int(values['-DECADE-'])
        output['search']: bool = not values['-SEARCH_TYPE-']

        gamesFound = database.Search(output['genres'], output['languages'], output['tags'], output['date'], output['developers'], output['publishers'],
                                     output['reviews'], output['name'], output['price_min'], output['price_max'], output['decades'], output['id'], output['search'])
        fields = ["AppID", "Nome", "Desenvolvedor", "Publicador", "Data de Lançamento", "Tags", "Preço", "Reviews"]

        self.showGames(gamesFound, fields)

    def showGames(self, gamesFound, fields: list[str]):
        """Mostra os jogos encontrados na janela de resultados"""
        # ------ Table Definition ------
        layout = [[sg.Table(values=gamesFound, headings=fields, max_col_width=20,
                            auto_size_columns=True,
                            # cols_justification=('left','center','right','c', 'l', 'bad'),       # Added on GitHub only as of June 2022
                            display_row_numbers=False,
                            justification='center',
                            num_rows=20,
                            alternating_row_color='lightblue',
                            key='-TABLE-',
                            selected_row_colors='red on yellow',
                            enable_events=True,
                            expand_x=False,
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

        window.close()

    def cleanFields(self, window: sg.Window, fields: dict):
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
        window['-REV_EX_POS-'].update(False)
        window['-REV_MUITO_POS-'].update(False)
        window['-REV_POS-'].update(False)
        window['-REV_LIG_POS-'].update(False)
        window['-REV_NEUTRAS-'].update(False)
        window['-REV_LIG_NEG-'].update(False)
        window['-REV_NEG-'].update(False)
        window['-REV_MUITO_NEG-'].update(False)
        window['-REV_EX_NEG-'].update(False)
        window['-DECADE-'].update(values=fields["decades"])
        window['-SEARCH_TYPE-'].update(1)

    def create_search(self, fields: dict) -> list:
        """Layout da janela principal"""
        return [[sg.Text('STEAM BUG'),],
                [
                sg.Text('GENERO', justification="center",
                        size=self.block_column_size-3),
                sg.Text('IDIOMA', justification="center",
                        size=self.block_column_size-3),
                sg.Text('TAGS', justification="center",
                        size=self.block_column_size-3),
                sg.Text('DATA', justification="center",
                        size=self.block_column_size-3),
                sg.Text('DEV', justification="center",
                        size=self.block_column_size-3),
                sg.Text('PUBLISHER', justification="center",
                        size=self.block_column_size-3)
                ],
                [
                sg.Listbox(values=fields["genres"], select_mode=sg.SELECT_MODE_MULTIPLE, size=(
                    self.block_column_size - 3, self.block_line_size), key='-GENERO-'),
                sg.Listbox(values=fields["languages"], select_mode=sg.SELECT_MODE_MULTIPLE, size=(
                    self.block_column_size - 3, self.block_line_size), key='-IDIOMA-'),
                sg.Listbox(values=fields["tags"], select_mode=sg.SELECT_MODE_MULTIPLE, size=(
                    self.block_column_size - 3, self.block_line_size), key='-TAGS-'),
                sg.Listbox(values=fields["dates"], select_mode=sg.SELECT_MODE_MULTIPLE, size=(
                    self.block_column_size - 3, self.block_line_size), key='-DATA-'),
                sg.Listbox(values=fields["developers"], select_mode=sg.SELECT_MODE_MULTIPLE, size=(
                    self.block_column_size - 3, self.block_line_size), key='-DEV-'),
                sg.Listbox(values=fields["publishers"], select_mode=sg.SELECT_MODE_MULTIPLE, size=(
                    self.block_column_size - 3, self.block_line_size), key='-PUBLISHER-')
                ],
                [
                sg.Push(), sg.Text('Preço MIN:', justification="center"), sg.Push(), sg.Text(
                    'Preço MAX:', justification="center"), sg.Push()
                ],
                [
                sg.Slider(orientation="h", range=(0, self.max_price), resolution=0.01, key='-PRICE_MIN-'), sg.Slider(
                    orientation="h", range=(0, self.max_price), resolution=0.01, default_value=fields["price"], key='-PRICE_MAX-')
                ],
                [
                sg.Text('Década:'), sg.Combo(
                    values=fields["decades"], key='-DECADE-')
                ],
                [
                [sg.Text('Review:')],
                [
                    sg.Checkbox('Extremamente Positivas',
                                size=self.block_column_size, key='-REV_EX_POS-',),
                    sg.Checkbox(
                        'Muito Positivas', size=self.block_column_size, key='-REV_MUITO_POS-'),
                    sg.Checkbox(
                        'Positivas', size=self.block_column_size, key='-REV_POS-')
                ],
                [
                    sg.Checkbox('Ligeiramente Positivas',
                                size=self.block_column_size, key='-REV_LIG_POS-'),
                    sg.Checkbox(
                        'Neutras', size=self.block_column_size, key='-REV_NEUTRAS-'),
                    sg.Checkbox('Ligeiramente Negativas',
                                size=self.block_column_size, key='-REV_LIG_NEG-')
                ],
                [
                    sg.Checkbox(
                        'Negativas', size=self.block_column_size, key='-REV_NEG-'),
                    sg.Checkbox(
                        'Muito Negativas', size=self.block_column_size, key='-REV_MUITO_NEG-'),
                    sg.Checkbox('Extremamente Negativas',
                                size=self.block_column_size, key='-REV_EX_NEG-')
                ]
                ],
                [
                sg.Text('AppID:'), sg.InputText(key='-ID-', size=10),
                sg.Text('Nome:'), sg.InputText(key='-NAME-')
                ],
                [
                sg.Button('BUSCAR'),
                sg.Button('Limpar Filtros'),
                sg.Checkbox('Busca de Overlap',
                            size=self.block_column_size,default=1, key='-SEARCH_TYPE-')
                ]
                ]
