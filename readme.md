Příkazy nad množinami

Příkaz pracuje nad množinami a jeho výsledkem je buď množina (v tom případě tiskne množinu ve stejném formátu jako se očekává ve vstupním souboru, tj. začíná "S " a pokračuje mezerou oddělenými prvky) nebo je výsledkem pravdivostní hodnota (v tom případě tiskne true nebo false na samostatný řádek) nebo je výsledkem přirozené číslo (které se tiskne na samostatný řádek).

    empty A - tiskne true nebo false podle toho, jestli je množina definovaná na řádku A prázdná nebo neprázdná.
    card A - tiskne počet prvků v množině A (definované na řádku A).
    complement A - tiskne doplněk množiny A.
    union A B - tiskne sjednocení množin A a B.
    intersect A B - tiskne průnik množin A a B.
    minus A B - tiskne rozdíl množin A \ B.
    subseteq A B - tiskne true nebo false podle toho, jestli je množina A podmnožinou množiny B.
    subset A B - tiskne true nebo false, jestli je množina A vlastní podmnožina množiny B.
    equals A B - tiskne true nebo false, jestli jsou množiny rovny.

Příkazy nad relacemi

Příkaz pracuje nad relacemi a jeho výsledkem je buď pravdivostní hodnota (tiskne true nebo false), nebo množina (tiskne množinu ve formátu jako ve vstupnímu souboru).

    reflexive R - tiskne true nebo false, jestli je relace reflexivní.
    symmetric R - tiskne true nebo false, jestli je relace symetrická.
    antisymmetric R - tiskne true nebo false, jestli je relace antisymetrická.
    transitive R - tiskne true nebo false, jestli je relace tranzitivní.
    function R - tiskne true nebo false, jestli je relace R funkcí.
    domain R - tiskne definiční obor funkce R (lze aplikovat i na relace - první prvky dvojic).
    codomain R - tiskne obor hodnot funkce R (lze aplikovat i na relace - druhé prvky dvojic).
    injective R - tiskne true nebo false, jestli je funkce R injektivní.
    surjective R - tiskne true nebo false, jestli je funkce R surjektivní.
    bijective R - tiskne true nebo false, jestli je funkce R bijektivní.

Implementační detaily

    Maximální podporovaný počet řádků je 1000.
    Na pořadí prvků v množině a v relaci na výstupu nezáleží.
    Všechny prvky množin a v relacích musí patřit do univerza. Pokud se prvek v množině nebo dvojice v relaci opakuje, jedná se o chybu.
