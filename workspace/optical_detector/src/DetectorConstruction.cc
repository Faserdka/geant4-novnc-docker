#include "DetectorConstruction.hh"
#include "PhotocathodeSD.hh"

#include "G4Material.hh"
#include "G4NistManager.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"
#include "G4SDManager.hh"
#include "G4LogicalSkinSurface.hh"
#include "G4OpticalSurface.hh"

// Конструктор и деструктор по умолчанию
DetectorConstruction::DetectorConstruction() : G4VUserDetectorConstruction() {}
DetectorConstruction::~DetectorConstruction() {}

// Основной метод для построения геометрии и материалов детектора
G4VPhysicalVolume* DetectorConstruction::Construct() {
  // Получаем менеджер базы данных материалов NIST
  G4NistManager* nist = G4NistManager::Instance();

  // --- ОПРЕДЕЛЕНИЕ МАТЕРИАЛОВ ---
  // Стандартный воздух
  G4Material* air = nist->FindOrBuildMaterial("G4_AIR");
  // Стекло типа Pyrex для сосуда и PMT
  G4Material* glass = nist->FindOrBuildMaterial("G4_Pyrex_Glass");
  // Вакуум для внутренней части ФЭУ (PMT)
  G4Material* vacuum = nist->FindOrBuildMaterial("G4_Galactic");
  
  // Кастомные материалы или на базе существующих:
  // Сцинтиллятор LAB (Линейный алкилбензол). Для простоты за основу взята вода G4_WATER.
  G4Material* lab = nist->FindOrBuildMaterial("G4_WATER"); 
  
  // Оптическая смазка между сосудом и ФЭУ (Приближение: диоксид кремния)
  G4Material* grease = nist->FindOrBuildMaterial("G4_SILICON_DIOXIDE"); // Approximation

  // Материал фотокатода - вакуум со специальными оптическими свойствами
  G4Material* pc_mat = vacuum;

  // --- ОПРЕДЕЛЕНИЕ ОПТИЧЕСКИХ СВОЙСТВ МАТЕРИАЛОВ ---
  // Задаем диапазон энергий фотонов для оптических свойств (от 2.0 eV до 3.0 eV)
  G4double energy[2] = {2.0*eV, 3.0*eV};
  
  // Оптические свойства воздуха (показатель преломления = 1.0)
  G4double rindex_air[2] = {1.0, 1.0};
  G4MaterialPropertiesTable* mpt_air = new G4MaterialPropertiesTable();
  mpt_air->AddProperty("RINDEX", energy, rindex_air, 2);
  air->SetMaterialPropertiesTable(mpt_air);

  // Оптические свойства LAB (показатель преломления = 1.49, длина поглощения = 10 м)
  G4double rindex_lab[2] = {1.49, 1.49};
  G4double abs_lab[2] = {10.0*m, 10.0*m};
  G4MaterialPropertiesTable* mpt_lab = new G4MaterialPropertiesTable();
  mpt_lab->AddProperty("RINDEX", energy, rindex_lab, 2);
  mpt_lab->AddProperty("ABSLENGTH", energy, abs_lab, 2);
  lab->SetMaterialPropertiesTable(mpt_lab);

  // Оптические свойства стекла Pyrex (показатель преломления = 1.5, длина поглощения = 3 м)
  G4double rindex_glass[2] = {1.50, 1.50};
  G4double abs_glass[2] = {3.0*m, 3.0*m};
  G4MaterialPropertiesTable* mpt_glass = new G4MaterialPropertiesTable();
  mpt_glass->AddProperty("RINDEX", energy, rindex_glass, 2);
  mpt_glass->AddProperty("ABSLENGTH", energy, abs_glass, 2);
  glass->SetMaterialPropertiesTable(mpt_glass);

  // Оптические свойства смазки (показатель преломления = 1.45)
  G4double rindex_grease[2] = {1.45, 1.45};
  G4MaterialPropertiesTable* mpt_grease = new G4MaterialPropertiesTable();
  mpt_grease->AddProperty("RINDEX", energy, rindex_grease, 2);
  grease->SetMaterialPropertiesTable(mpt_grease);

  // Оптические свойства вакуума (показатель преломления = 1.0)
  G4double rindex_vacuum[2] = {1.0, 1.0};
  G4MaterialPropertiesTable* mpt_vacuum = new G4MaterialPropertiesTable();
  mpt_vacuum->AddProperty("RINDEX", energy, rindex_vacuum, 2);
  vacuum->SetMaterialPropertiesTable(mpt_vacuum);

  // --- ПОСТРОЕНИЕ ГЕОМЕТРИЧЕСКИХ ОБЪЕМОВ ---
  
  // 1. Объем Мира (World)
  // Создание формы: коробка 50см x 50см x 50см
  G4Box* solidWorld = new G4Box("World", 25.*cm, 25.*cm, 25.*cm);
  // Создание логического объема: форма + материал (воздух)
  G4LogicalVolume* logicWorld = new G4LogicalVolume(solidWorld, air, "World");
  // Создание физического объема: размещение в пространстве
  G4VPhysicalVolume* physWorld = new G4PVPlacement(0, G4ThreeVector(), logicWorld, "World", 0, false, 0, true);

  // 2. Сосуд (Vessel)
  // Форма цилиндра: внутренний радиус=0, внешний=5.2см, полувысота=10.1см
  G4Tubs* solidVessel = new G4Tubs("Vessel", 0., 5.2*cm, 10.1*cm, 0., 360.*deg);
  G4LogicalVolume* logicVessel = new G4LogicalVolume(solidVessel, glass, "Vessel");
  new G4PVPlacement(0, G4ThreeVector(0,0,-0.1*cm), logicVessel, "Vessel", logicWorld, false, 0, true);

  // 3. Объем LAB внутри сосуда
  // Форма цилиндра: внешний радиус=5.0см, полувысота=10.0см
  G4Tubs* solidLAB = new G4Tubs("LAB", 0., 5.0*cm, 10.0*cm, 0., 360.*deg);
  G4LogicalVolume* logicLAB = new G4LogicalVolume(solidLAB, lab, "LAB");
  new G4PVPlacement(0, G4ThreeVector(0,0,0.1*cm), logicLAB, "LAB", logicVessel, false, 0, true);

  // 4. Оптическая смазка (Grease)
  // Очень тонкий цилиндр: радиус=5.0см, полувысота=0.05см
  G4Tubs* solidGrease = new G4Tubs("Grease", 0., 5.0*cm, 0.05*cm, 0., 360.*deg);
  G4LogicalVolume* logicGrease = new G4LogicalVolume(solidGrease, grease, "Grease");
  new G4PVPlacement(0, G4ThreeVector(0,0,10.05*cm), logicGrease, "Grease", logicWorld, false, 0, true);

  // 5. Стекло ФЭУ (PMT Glass)
  // Форма цилиндра: внешний радиус=5.0см, полувысота=5.0см
  G4Tubs* solidPMTGlass = new G4Tubs("PMTGlass", 0., 5.0*cm, 5.0*cm, 0., 360.*deg);
  G4LogicalVolume* logicPMTGlass = new G4LogicalVolume(solidPMTGlass, glass, "PMTGlass");
  new G4PVPlacement(0, G4ThreeVector(0,0,15.1*cm), logicPMTGlass, "PMTGlass", logicWorld, false, 0, true);

  // 6. Вакуум внутри ФЭУ (PMT Vacuum)
  // Форма цилиндра: внешний радиус=4.8см, полувысота=4.8см
  G4Tubs* solidPMTVacuum = new G4Tubs("PMTVacuum", 0., 4.8*cm, 4.8*cm, 0., 360.*deg);
  G4LogicalVolume* logicPMTVacuum = new G4LogicalVolume(solidPMTVacuum, vacuum, "PMTVacuum");
  new G4PVPlacement(0, G4ThreeVector(0,0,0), logicPMTVacuum, "PMTVacuum", logicPMTGlass, false, 0, true);

  // 7. Фотокатод (Photocathode)
  // Ультратонкий диск: полувысота=0.5нм (0.0005 мкм)
  G4Tubs* solidPhotocathode = new G4Tubs("Photocathode", 0., 4.8*cm, 0.5*nm, 0., 360.*deg);
  G4LogicalVolume* logicPhotocathode = new G4LogicalVolume(solidPhotocathode, pc_mat, "Photocathode");
  new G4PVPlacement(0, G4ThreeVector(0,0,-4.8*cm + 0.5*nm), logicPhotocathode, "Photocathode", logicPMTVacuum, false, 0, true);

  // --- ОПРЕДЕЛЕНИЕ ОПТИЧЕСКОЙ ПОВЕРХНОСТИ ---
  // Описываем оптическую поверхность сосуда (отражение/преломление на границе сред)
  G4OpticalSurface* opSurface = new G4OpticalSurface("VesselSurface");
  opSurface->SetType(dielectric_metal);
  opSurface->SetFinish(ground);
  opSurface->SetModel(unified);

  G4MaterialPropertiesTable* smpt = new G4MaterialPropertiesTable();
  G4double reflectivity[2] = {0.95, 0.95};
  smpt->AddProperty("REFLECTIVITY", energy, reflectivity, 2);
  opSurface->SetMaterialPropertiesTable(smpt);

  // Применяем свойства поверхности к оболочке логического объема Vessel
  new G4LogicalSkinSurface("VesselSkin", logicVessel, opSurface);

  return physWorld;
}

// Метод инициализации чувствительного детектора
void DetectorConstruction::ConstructSDandField() {
  // Создаем объект чувствительного детектора PhotocathodeSD
  PhotocathodeSD* sd = new PhotocathodeSD("PhotocathodeSD");
  // Регистрируем его в менеджере чувствительных объемов Geant4
  G4SDManager::GetSDMpointer()->AddNewDetector(sd);
  // Привязываем созданный детектор к логическому объему "Photocathode"
  SetSensitiveDetector("Photocathode", sd);
}
