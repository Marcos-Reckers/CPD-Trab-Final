import PySimpleGUI as sg
import database
import images


class windows:
    def __init__(self, Colum_size: int = 18, Line_size: int = 10) -> None:
        self.block_column_size = Colum_size  # Tamanho das colunas
        self.block_line_size = Line_size  # Tamanho das linhas
        self.icon = images.ICON
        sg.change_look_and_feel('DarkGray13')

    def search(self):
        fields: dict = {}
        fields["languages"] = self.fields('language')
        fields["genres"] = self.fields('genre')
        fields["tags"] = self.fields('tag')
        fields["developers"] = self.fields('developer')
        fields["publishers"] = self.fields('publisher')
        fields["dates"] = self.fields('date')
        fields["price"] = self.fields('price')[0]
        fields["decades"] = self.fields('decade')

        self.max_price = fields["price"]

        search_screen = self.create_search(fields)

        window = sg.Window(
            'Vapor: Pesquisa de Jogos',
            search_screen,
            element_justification='center',
            resizable=True,
            icon=self.icon
        )

        while True:
            event, values = window.read()
            if event == sg.WIN_CLOSED:
                break
            elif event == 'Buscar':
                values['-SEARCH_TYPE-'] = window['-SEARCH_TYPE-'].metadata
                for value in window.AllKeysDict:
                    if '-REV' in value:
                        values[value] = window[value].metadata
                self.searchDatabase(values)
            elif event == 'Limpar':
                self.cleanFields(window, fields)
            elif event == '-SEARCH_TYPE-':  # if the graphical button that changes images
                window['-SEARCH_TYPE-'].metadata = not window['-SEARCH_TYPE-'].metadata
                window['-SEARCH_TYPE-'].update(
                    image_data=self.toggle_btn_on if window['-SEARCH_TYPE-'].metadata else self.toggle_btn_off)
            elif '-REV' in event:
                window[event].metadata = not window[event].metadata
                window[event].update(
                    self.checked if window[event].metadata else self.unchecked)
            elif event == 'Adicionar':
                self.addDatabase(window, fields)
            elif event in (sg.WIN_CLOSED, 'Exit'):     # If user closed the window
                break

    def fail_popup(self, title: str, text: str):
        """Cria a janela de erro"""
        sg.Window(title, [[sg.Text(text, justification='c')], [sg.Button("Continuar")]], text_justification='c', element_justification='c', icon=self.icon).read(close=True)

    def create_DB(self) -> str:
        """Cria a janela para procurar e criar a base de dados"""
        create_DB_screen = [
            [sg.Text('Caminho para DB')],
            [sg.Combo(sg.user_settings_get_entry('-filenames-', []), default_value=sg.user_settings_get_entry(
                '-last filename-', ''), size=(80, 1), key='-FILENAME-'), sg.FileBrowse()],
            [sg.Button('Go'), sg.Button('Exit')]
        ]

        event, values = sg.Window(
            "Vapor: Criação da Base de Dados", create_DB_screen,
            icon=self.icon).read(close=True)

        if event == 'Go':
            sg.user_settings_set_entry(
                '-filenames-', list(set(sg.user_settings_get_entry('-filenames-', []) + [values['-FILENAME-'], ])))
            sg.user_settings_set_entry('-last filename-', values['-FILENAME-'])
        elif event in (sg.WIN_CLOSED, 'Exit'):     # If user closed the window
            return 'exit'

        return values['-FILENAME-']

    def addDatabase(self, ParWindow: sg.Window, fields: dict):
        """Cria a janela para adicionar um jogo"""
        TITULOS = ('AppID', 'Nome', 'Desenvolvedor', 'Editora', 'Data de Lançamento',
                   'Tags Populares', 'Detalhes do Jogo', 'Linguagens', 'Género', 'Preço', 'Review')

        layout = [[sg.Text(
            "Insire os dados de um jogo OU um caminho para um ficheiro .csv", font='Default 13')]]
        layout += [[sg.Combo(sg.user_settings_get_entry('-filenames-', []),
                             default_value='', size=(80, 1), key='-FILENAME-'), sg.FileBrowse()]]

        layout += [[sg.Text(' ')]]

        layout += [[sg.Text(string, enable_events=True, pad=(12, 1))
                    for index, string in enumerate(TITULOS)]]

        layout += [[sg.Input('', justification='l', key=string, size=(12, 1))
                    for index, string in enumerate(TITULOS)]]

        layout += [[sg.Text(' ')]]

        layout += [[sg.Button('Go'), sg.Button('Exit')]]

        # ------ Create Window ------
        window = sg.Window('Vapor: Adição de Jogos', layout, default_element_size=(
            12, 1), element_padding=(1, 1), return_keyboard_events=True, resizable=True,
            icon=self.icon)

        # ------ Event Loop ------
        current_cell = 0
        while True:  # Event Loop
            event, values = window.read()
            elem: sg.Element = window.find_element_with_focus()
            if (elem is not None):
                current_cell: int = TITULOS.index(
                    elem.key) if elem.key in TITULOS else current_cell
            c = current_cell

            if event in (sg.WIN_CLOSED, 'Exit'):     # If user closed the window
                break

            if event.startswith('Tab'):
                c = c + 1 * (c < len(TITULOS) - 1)

            # if the current cell changed, set focus on new cell
            if current_cell != c:
                current_cell = c
                # set the focus on the element moved to
                window[TITULOS[current_cell]].set_focus()
                # when setting focus, also highlight the data in the element so typing overwrites
                window[TITULOS[current_cell]].update(select=True)

            if event.startswith('Go'):
                path = values['-FILENAME-']
                appid = values['AppID']
                name = values['Nome']
                developer = values['Desenvolvedor']
                publisher = values['Editora']
                release_date = values['Data de Lançamento']
                popular_tags = values['Tags Populares']
                game_details = values['Detalhes do Jogo']
                languages = values['Linguagens']
                genre = values['Género']
                price = values['Preço']
                review = values['Review']
                ret = database.Append(path, int(appid) if appid else 0, name, developer, publisher,
                                      release_date, popular_tags, game_details, languages, genre, price, review)

                fields["languages"] = self.fields('language', ret)
                fields["genres"] = self.fields('genre', ret)
                fields["tags"] = self.fields('tag', ret)
                fields["developers"] = self.fields('developer', ret)
                fields["publishers"] = self.fields('publisher', ret)
                fields["dates"] = self.fields('date', ret)
                fields["price"] = self.fields('price', ret)[0]
                fields["decades"] = self.fields('decade', ret)
                self.cleanFields(ParWindow, fields)
                break

        window.close()

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
        # print(output)

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
        window = sg.Window('Vapor: Resultados da Busca', layout,
                           # ttk_theme='clam',
                           # font='Helvetica 25',
                           resizable=True,
                           icon=self.icon
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
                        gamesFound.sort(
                            key=lambda x: x[col_clicked], reverse=order)
                        window['-TABLE-'].update(values=gamesFound)

        window.close()

    def cleanFields(self, window: sg.Window, fields: dict):
        def cleanButtonEvent(window: sg.Window, key: str, default: bool, btn_on: bytes, btn_off: bytes):
            window[key].metadata = default
            window[key].update(btn_on if window[key].metadata else btn_off)

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
        cleanButtonEvent(window, '-REV_EX_POS-', False,
                         self.checked, self.unchecked)
        cleanButtonEvent(window, '-REV_MUITO_POS-', False,
                         self.checked, self.unchecked)
        cleanButtonEvent(window, '-REV_POS-', False,
                         self.checked, self.unchecked)
        cleanButtonEvent(window, '-REV_LIG_POS-', False,
                         self.checked, self.unchecked)
        cleanButtonEvent(window, '-REV_NEUTRAS-', False,
                         self.checked, self.unchecked)
        cleanButtonEvent(window, '-REV_LIG_NEG-', False,
                         self.checked, self.unchecked)
        cleanButtonEvent(window, '-REV_NEG-', False,
                         self.checked, self.unchecked)
        cleanButtonEvent(window, '-REV_MUITO_NEG-', False,
                         self.checked, self.unchecked)
        cleanButtonEvent(window, '-REV_EX_NEG-', False,
                         self.checked, self.unchecked)
        window['-SEARCH_TYPE-'].metadata = True
        window['-SEARCH_TYPE-'].update(
            image_data=self.toggle_btn_on if window['-SEARCH_TYPE-'].metadata else self.toggle_btn_off)

    def create_search(self, fields: dict) -> list:
        """Layout da janela principal"""
        self.toggle_btn_off = images.TOGGLE_BTN_OFF
        self.toggle_btn_on = images.TOGGLE_BTN_ON
        self.checked = images.CHECKED
        self.unchecked = images.UNCHECKED
        return [
                [
                sg.Text('Generos', justification="center",
                        size=self.block_column_size-3, expand_x=True),
                sg.Text('Idiomas', justification="center",
                        size=self.block_column_size-3, expand_x=True),
                sg.Text('Tags', justification="center",
                        size=self.block_column_size-3, expand_x=True),
                sg.Text('Ano', justification="center",
                        size=self.block_column_size-3, expand_x=True),
                sg.Text('Desenvolvedor', justification="center",
                        size=self.block_column_size-3, expand_x=True),
                sg.Text('Editora', justification="center",
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
                sg.Push(), sg.Text('Preço Minimo', justification="center", expand_x=True), sg.Push(), sg.Text(
                    'Preço Maximo', justification="center", expand_x=True), sg.Push()
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
                sg.Button('Buscar'),
                sg.Button('Limpar'),
                [sg.Text('Tipo de Busca:')],
                [sg.Text('Inclusivo'),
                    sg.Button(image_data=self.toggle_btn_on, key='-SEARCH_TYPE-', button_color=(
                        sg.theme_background_color(), sg.theme_background_color()), border_width=0, metadata=True),
                    sg.Text('Exclusivo')],
                sg.Button('Adicionar')
                ]
                ]

    def fields(self, field: str, data: list[list[str]] = None) -> list:
        database_index = 0
        match field:
            case 'genre':
                database_field = 'gen'
                database_index = 1
            case 'language':
                database_field = 'lan'
                database_index = 0
            case 'tag':
                database_field = 'tag'
                database_index = 4
            case 'developer':
                database_field = 'dev'
                database_index = 2
            case 'publisher':
                database_field = 'pub'
                database_index = 3
            case 'date':
                database_field = 'dat'
                database_index = 5
            case 'price':
                database_field = 'pri'
                database_index = 6
            case 'decade':
                database_field = 'dat'
                database_index = 5

        out = database.GetField(
            database_field) if data is None else data[database_index]
        sortedFields = sorted(out)

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


if __name__ == "__main__":
    print("Programa de GUI não pode ser executado diretamente!")
