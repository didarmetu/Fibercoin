# Fibercoin Translation Process

Fibercoin supports multiple languages through the Qt translation system.

Translation source files are stored in:

    src/qt/locale/

They use names such as:

    fibercoin_en.ts
    fibercoin_de.ts
    fibercoin_es.ts
    fibercoin_pt_BR.ts

Compiled translation files use the `.qm` extension.

## Translation source

The primary English translation source is:

    src/qt/locale/fibercoin_en.ts

Strings from both Qt and non-Qt Fibercoin source code are collected into the translation system.

The generated non-Qt translation source is:

    src/qt/fibercoinstrings.cpp

This file is generated from translatable strings in the Fibercoin core source.

## Required tools

Updating translations requires Qt translation tools, including:

    lupdate
    lrelease

The translation extraction process also uses standard build tools and gettext where required.

On Ubuntu or Debian, gettext can normally be installed with:

    sudo apt-get install gettext

Qt development tools must also be installed.

## Regenerating the English translation source

Configure Fibercoin first so the Qt build tools are detected.

Then run:

    cd src
    make translate

The translation target regenerates `qt/fibercoinstrings.cpp` and updates:

    qt/locale/fibercoin_en.ts

using Qt `lupdate`.

## Compiled translations

Qt translation source files use the `.ts` format.

During the build, `lrelease` converts them into binary `.qm` files used by the Fibercoin Qt wallet.

The translation resource manifest is:

    src/qt/fibercoin_locale.qrc

For example:

    <qresource prefix="/translations">
        <file alias="en">locale/fibercoin_en.qm</file>
    </qresource>

New languages must be added to this resource file.

## Writing translatable Qt code

Qt interface strings should use `tr()`.

Example:

    QToolBar* toolbar = addToolBar(tr("Tabs toolbar"));

Do not hard-code user-visible text when it should be translated.

## Updating translations

When user-visible source strings change:

1. Configure the project with Qt support.
2. Run:

       cd src
       make translate

3. Review the changes to:

       src/qt/fibercoinstrings.cpp
       src/qt/locale/fibercoin_en.ts

4. Update translated `.ts` files as needed.
5. Build the Qt application and verify that the `.qm` resources are generated correctly.

Translation updates are commonly grouped near release preparation to reduce unnecessary translation conflicts.

## Plural translations

Qt uses `%n` for plural-aware strings.

When adding or changing plural strings:

1. Regenerate `fibercoin_en.ts`.
2. Open the translation in Qt Linguist.
3. Search for `%n`.
4. Complete the required singular and plural forms.
5. Mark the translation as complete.
6. Save the `.ts` file.

Different languages may require more than two plural forms.

## Adding a new language

Create a translation source file using the appropriate locale name:

    fibercoin_xx.ts

or:

    fibercoin_xx_YY.ts

Then add the compiled `.qm` resource to:

    src/qt/fibercoin_locale.qrc

Example:

    <file alias="de">locale/fibercoin_de.qm</file>

The resource entry must reference the compiled `.qm` file rather than the source `.ts` file.

## Qt project support

The Qt project configuration also includes the translation resources:

    contrib/fibercoin-qt.pro

Additional translation tooling is available in:

    contrib/qt_translations.py

## Release validation

Before a Fibercoin release:

- regenerate the English translation source
- review changed source strings
- update affected translations
- compile the Qt translation resources
- build the Qt wallet
- verify that translated interfaces load correctly
