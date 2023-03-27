import PySimpleGUI as sg
import subprocess
import os


class database:
    def __init__(self, DB_PATH: str):
        self.DB_PATH = DB_PATH

    def Validate(self) -> bool:
        validate = subprocess.run([self.DB_PATH, '-v'], capture_output=True)
        if '0' in validate.stdout.decode('utf-8'):
            return False
        else:
            return True

    def CheckExeExistence(self) -> bool:
        return not os.path.isfile(self.DB_PATH)

    def create(self, path: str) -> bool:
        saida_c = subprocess.run(
            [self.DB_PATH, '-c', path, '-1'], capture_output=True)
        print(saida_c.stdout.decode('utf-8'))
        if saida_c.returncode == 0:
            return True
        else:
            return False

    def fields(self, field: str) -> list:
        match field:
            case 'genre':
                database_field = 'gen'
            case 'language':
                database_field = 'lan'
            case 'tag':
                database_field = 'tag'
            case 'developer':
                database_field = 'dev'
            case 'publisher':
                database_field = 'pub'
            case 'date':
                database_field = 'dat'
            case 'price':
                database_field = 'pri'
            case 'decade':
                database_field = 'dat'

        ret = subprocess.run([self.DB_PATH, '-f', database_field],
                             capture_output=True).stdout.decode('utf-8')

        fields = ret.split(';')
        sortedFields = sorted(fields)

        while '' in sortedFields:
            sortedFields.remove('')
        while '\r\n' in sortedFields:
            sortedFields.remove('\r\n')

        if field == 'decade':
            decade: int = int(sortedFields[0])
            offsetDec = decade % 10
            initialDec = decade - offsetDec
            # -3 para ignorar TBA e NaN
            finalDec = int(sortedFields[len(sortedFields)-3]) + 10
            return list(range(initialDec, finalDec, 10))
        elif field == 'price':
            price: int = 0
            for i in sortedFields:
                if i == '' or i == '\r\n':
                    continue
                num = int(i)
                if num > price:
                    price = num
            max_price = float(price) / 100
            return [max_price]

        return sortedFields


class Program:
    def __init__(self, DB_PATH: str) -> None:
        self.database = database(DB_PATH)
        self.windowManager = windows()

    def run(self):
        securityChecks = True
        while securityChecks:
            if self.database.CheckExeExistence():
                return self.windowManager.fail_popup(text='O programa de base de dados não existe')

            if not self.database.Validate():
                csv_path = self.windowManager.create_DB()
                self.database.create(csv_path)
                continue

            securityChecks = False

        fields: dict = {}
        fields["languages"] = self.database.fields('language')
        fields["genres"] = self.database.fields('genre')
        fields["tags"] = self.database.fields('tag')
        fields["developers"] = self.database.fields('developer')
        fields["publishers"] = self.database.fields('publisher')
        fields["dates"] = self.database.fields('date')
        fields["price"] = self.database.fields('price')[0]
        fields["decades"] = self.database.fields('decade')

        self.windowManager.search(fields)


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

    def create_DB(self) -> bool:
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
        output['name'] = values['-NAME-']
        output['id'] = values['-ID-']
        output['date'] = values['-DATA-']
        output['price_min'] = -1 if float(values['-PRICE_MIN-']
                                          ) == 0 else int(float(values['-PRICE_MIN-']) * 100)
        output['price_max'] = -1 if float(values['-PRICE_MAX-']) == self.max_price else int(
            float(values['-PRICE_MAX-']) * 100)
        output['genres'] = values['-GENERO-']
        output['languages'] = values['-IDIOMA-']
        output['tags'] = values['-TAGS-']
        output['developers'] = values['-DEV-']
        output['publishers'] = values['-PUBLISHER-']
        output['reviews'] = self.translate_reviews([values['-REV_EX_POS-'], values['-REV_MUITO_POS-'], 
                                                    values['-REV_POS-'], values['-REV_LIG_POS-'],
                                                    values['-REV_NEUTRAS-'], values['-REV_LIG_NEG-'], 
                                                    values['-REV_NEG-'], values['-REV_MUITO_NEG-'], 
                                                    values['-REV_EX_NEG-']])
        output['decades'] = values['-DECADE-']
        output['search']: int = 0 if values['-SEARCH_TYPE-'] else 1

        output = f"gen={output['genres']};lan={output['languages']};tag={output['tags']};dat={output['date']};dev={output['developers']};pub={output['publishers']};min={output['price_min']};max={output['price_max']};dec={output['decades']};rev={output['reviews']};gid={output['id']};nam={output['name']};typ={output['search']}"
    
        saida_c = subprocess.run(
            ['./code/database.exe', '-s', output], capture_output=True)
        gamesFound = saida_c.stdout.decode('utf-8').split('\n')

        games: list[list[str]] = []

        for game in gamesFound:
            i = game.split(';')
            if not i == ['']:
                games.append(i)

        fields = ["AppID", "Nome", "Desenvolvedor", "Publicador", "Data de Lançamento", "Tags", "Preço", "Reviews"]

        self.showGames(games, fields)

    def showGames(self, gamesFound: list[list[str]], fields: list[str]):
        """Mostra os jogos encontrados na janela de resultados"""
        # ------ Table Definition ------

        layout = [[sg.Table(values=gamesFound[:], headings=fields, max_col_width=20,
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


main = Program("code\database.exe")

main.run()
