from pyasn1.type import univ, namedtype, tag


class AsnPubKey(univ.Sequence):
    componentType = namedtype.NamedTypes(
        namedtype.NamedType('modulus', univ.Integer()),
        namedtype.NamedType('publicExponent', univ.Integer()),
    )
