#ifndef QPLACEALTERNATIVEVALUE_H
#define QPLACEALTERNATIVEVALUE_H

#include <QSharedDataPointer>
#include <QString>
#include "qmobilityglobal.h"

QTM_BEGIN_NAMESPACE

class QPlaceAlternativeValuePrivate;

class Q_LOCATION_EXPORT QPlaceAlternativeValue
{
public:
    enum TextSemantics {
        Synonim,
        Exonym,
        Unclassified
    };

    enum NameType {
        BaseName,
        ShortBaseName,
        Abbrevation,
        Unknown
    };

    QPlaceAlternativeValue();
    QPlaceAlternativeValue(const QPlaceAlternativeValue &other);

    virtual ~QPlaceAlternativeValue();

    QPlaceAlternativeValue &operator=(const QPlaceAlternativeValue &other);

    bool operator==(const QPlaceAlternativeValue &other) const;
    bool operator!=(const QPlaceAlternativeValue &other) const {
        return !(other == *this);
    }
    QString key() const;
    void setKey(const QString &data);
    QString language() const;
    void setLanguage(const QString &data);
    TextSemantics semantics() const;
    void setSemantics(const TextSemantics &data);
    NameType type() const;
    void setType(const NameType &data);
    QString value() const;
    void setValue(const QString &data);

private:
    QSharedDataPointer<QPlaceAlternativeValuePrivate> d;
};

QTM_END_NAMESPACE

#endif // QPLACEALTERNATIVEVALUE_H