TEMPLATE = subdirs

SUBDIRS += \
    RenamerCore \
    RenamerCoreTest \
    RenamerGUI

RenamerGUI.depends = RenamerCore
RenamerCoreTest.depends = RenamerCore
