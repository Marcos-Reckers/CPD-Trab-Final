import database
from GUI import windows

class Program:
    def __init__(self) -> None:
        self.windowManager = windows()

    def run(self):
        securityChecks = True
        while securityChecks:
            if not database.Validate():
                csv_path = self.windowManager.create_DB()
                time = database.Convert(csv_path, -1)
                print(time)
                continue

            securityChecks = False

        fields: dict = {}
        fields["languages"] = self.fields('language')
        fields["genres"] = self.fields('genre')
        fields["tags"] = self.fields('tag')
        fields["developers"] = self.fields('developer')
        fields["publishers"] = self.fields('publisher')
        fields["dates"] = self.fields('date')
        fields["price"] = self.fields('price')[0]
        fields["decades"] = self.fields('decade')

        self.windowManager.search(fields)

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

        out = database.GetField(database_field)
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

def main():
    main = Program()
    main.run()

if __name__ == "__main__":
    main()