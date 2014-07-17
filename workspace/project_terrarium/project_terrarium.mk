##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Debug_Linux
ProjectName            :=project_terrarium
ConfigurationName      :=Debug_Linux
IntermediateDirectory  :=./DebugLinux
OutDir                 := $(IntermediateDirectory)
WorkspacePath          := "/home/professor/svn-terrarium/workspace"
ProjectPath            := "/home/professor/svn-terrarium/workspace/project_terrarium"
CurrentFileName        :=
CurrentFilePath        :=
CurrentFileFullPath    :=
User                   :=Professor
Date                   :=09/11/2012
CodeLitePath           :="/home/professor/.codelite"
LinkerName             :=g++
ArchiveTool            :=ar rcus
SharedObjectLinkerName :=g++ -shared -fPIC
ObjectSuffix           :=.o
DependSuffix           :=.o.d
PreprocessSuffix       :=.o.i
DebugSwitch            :=-gstab
IncludeSwitch          :=-I
LibrarySwitch          :=-l
OutputSwitch           :=-o 
LibraryPathSwitch      :=-L
PreprocessorSwitch     :=-D
SourceSwitch           :=-c 
CompilerName           :=g++
C_CompilerName         :=gcc
OutputFile             :=$(IntermediateDirectory)/$(ProjectName)
Preprocessors          :=
ObjectSwitch           :=-o 
ArchiveOutputSwitch    := 
PreprocessOnlySwitch   :=-E 
ObjectsFileList        :="/home/professor/svn-terrarium/workspace/project_terrarium/project_terrarium.txt"
PCHCompileFlags        :=
MakeDirCommand         :=mkdir -p
CmpOptions             := -g -O0 -Wall $(Preprocessors)
C_CmpOptions           := -g -O0 -Wall $(Preprocessors)
LinkOptions            :=  `sdl-config --cflags --libs` -lSDL_image -lSDL_ttf -lSDL_mixer
IncludePath            :=  $(IncludeSwitch). $(IncludeSwitch). 
IncludePCH             := 
RcIncludePath          := 
Libs                   := 
LibPath                := $(LibraryPathSwitch). 


##
## User defined environment variables
##
CodeLiteDir:=/usr/share/codelite
Objects=$(IntermediateDirectory)/sdl$(ObjectSuffix) $(IntermediateDirectory)/main$(ObjectSuffix) $(IntermediateDirectory)/entity$(ObjectSuffix) $(IntermediateDirectory)/actor$(ObjectSuffix) $(IntermediateDirectory)/util$(ObjectSuffix) $(IntermediateDirectory)/block$(ObjectSuffix) $(IntermediateDirectory)/animated_entity$(ObjectSuffix) 

##
## Main Build Targets 
##
.PHONY: all clean PreBuild PrePreBuild PostBuild
all: $(OutputFile)

$(OutputFile): $(IntermediateDirectory)/.d $(Objects) 
	@$(MakeDirCommand) $(@D)
	@echo "" > $(IntermediateDirectory)/.d
	@echo $(Objects) > $(ObjectsFileList)
	$(LinkerName) $(OutputSwitch)$(OutputFile) @$(ObjectsFileList) $(LibPath) $(Libs) $(LinkOptions)

$(IntermediateDirectory)/.d:
	@test -d ./DebugLinux || $(MakeDirCommand) ./DebugLinux

PreBuild:


##
## Objects
##
$(IntermediateDirectory)/sdl$(ObjectSuffix): sdl.cpp $(IntermediateDirectory)/sdl$(DependSuffix)
	$(CompilerName) $(IncludePCH) $(SourceSwitch) "/home/professor/svn-terrarium/workspace/project_terrarium/sdl.cpp" $(CmpOptions) $(ObjectSwitch)$(IntermediateDirectory)/sdl$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/sdl$(DependSuffix): sdl.cpp
	@$(CompilerName) $(CmpOptions) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/sdl$(ObjectSuffix) -MF$(IntermediateDirectory)/sdl$(DependSuffix) -MM "/home/professor/svn-terrarium/workspace/project_terrarium/sdl.cpp"

$(IntermediateDirectory)/sdl$(PreprocessSuffix): sdl.cpp
	@$(CompilerName) $(CmpOptions) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/sdl$(PreprocessSuffix) "/home/professor/svn-terrarium/workspace/project_terrarium/sdl.cpp"

$(IntermediateDirectory)/main$(ObjectSuffix): main.cpp $(IntermediateDirectory)/main$(DependSuffix)
	$(CompilerName) $(IncludePCH) $(SourceSwitch) "/home/professor/svn-terrarium/workspace/project_terrarium/main.cpp" $(CmpOptions) $(ObjectSwitch)$(IntermediateDirectory)/main$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/main$(DependSuffix): main.cpp
	@$(CompilerName) $(CmpOptions) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/main$(ObjectSuffix) -MF$(IntermediateDirectory)/main$(DependSuffix) -MM "/home/professor/svn-terrarium/workspace/project_terrarium/main.cpp"

$(IntermediateDirectory)/main$(PreprocessSuffix): main.cpp
	@$(CompilerName) $(CmpOptions) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/main$(PreprocessSuffix) "/home/professor/svn-terrarium/workspace/project_terrarium/main.cpp"

$(IntermediateDirectory)/entity$(ObjectSuffix): entity.cpp $(IntermediateDirectory)/entity$(DependSuffix)
	$(CompilerName) $(IncludePCH) $(SourceSwitch) "/home/professor/svn-terrarium/workspace/project_terrarium/entity.cpp" $(CmpOptions) $(ObjectSwitch)$(IntermediateDirectory)/entity$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/entity$(DependSuffix): entity.cpp
	@$(CompilerName) $(CmpOptions) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/entity$(ObjectSuffix) -MF$(IntermediateDirectory)/entity$(DependSuffix) -MM "/home/professor/svn-terrarium/workspace/project_terrarium/entity.cpp"

$(IntermediateDirectory)/entity$(PreprocessSuffix): entity.cpp
	@$(CompilerName) $(CmpOptions) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/entity$(PreprocessSuffix) "/home/professor/svn-terrarium/workspace/project_terrarium/entity.cpp"

$(IntermediateDirectory)/actor$(ObjectSuffix): actor.cpp $(IntermediateDirectory)/actor$(DependSuffix)
	$(CompilerName) $(IncludePCH) $(SourceSwitch) "/home/professor/svn-terrarium/workspace/project_terrarium/actor.cpp" $(CmpOptions) $(ObjectSwitch)$(IntermediateDirectory)/actor$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/actor$(DependSuffix): actor.cpp
	@$(CompilerName) $(CmpOptions) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/actor$(ObjectSuffix) -MF$(IntermediateDirectory)/actor$(DependSuffix) -MM "/home/professor/svn-terrarium/workspace/project_terrarium/actor.cpp"

$(IntermediateDirectory)/actor$(PreprocessSuffix): actor.cpp
	@$(CompilerName) $(CmpOptions) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/actor$(PreprocessSuffix) "/home/professor/svn-terrarium/workspace/project_terrarium/actor.cpp"

$(IntermediateDirectory)/util$(ObjectSuffix): util.cpp $(IntermediateDirectory)/util$(DependSuffix)
	$(CompilerName) $(IncludePCH) $(SourceSwitch) "/home/professor/svn-terrarium/workspace/project_terrarium/util.cpp" $(CmpOptions) $(ObjectSwitch)$(IntermediateDirectory)/util$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/util$(DependSuffix): util.cpp
	@$(CompilerName) $(CmpOptions) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/util$(ObjectSuffix) -MF$(IntermediateDirectory)/util$(DependSuffix) -MM "/home/professor/svn-terrarium/workspace/project_terrarium/util.cpp"

$(IntermediateDirectory)/util$(PreprocessSuffix): util.cpp
	@$(CompilerName) $(CmpOptions) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/util$(PreprocessSuffix) "/home/professor/svn-terrarium/workspace/project_terrarium/util.cpp"

$(IntermediateDirectory)/block$(ObjectSuffix): block.cpp $(IntermediateDirectory)/block$(DependSuffix)
	$(CompilerName) $(IncludePCH) $(SourceSwitch) "/home/professor/svn-terrarium/workspace/project_terrarium/block.cpp" $(CmpOptions) $(ObjectSwitch)$(IntermediateDirectory)/block$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/block$(DependSuffix): block.cpp
	@$(CompilerName) $(CmpOptions) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/block$(ObjectSuffix) -MF$(IntermediateDirectory)/block$(DependSuffix) -MM "/home/professor/svn-terrarium/workspace/project_terrarium/block.cpp"

$(IntermediateDirectory)/block$(PreprocessSuffix): block.cpp
	@$(CompilerName) $(CmpOptions) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/block$(PreprocessSuffix) "/home/professor/svn-terrarium/workspace/project_terrarium/block.cpp"

$(IntermediateDirectory)/animated_entity$(ObjectSuffix): animated_entity.cpp $(IntermediateDirectory)/animated_entity$(DependSuffix)
	$(CompilerName) $(IncludePCH) $(SourceSwitch) "/home/professor/svn-terrarium/workspace/project_terrarium/animated_entity.cpp" $(CmpOptions) $(ObjectSwitch)$(IntermediateDirectory)/animated_entity$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/animated_entity$(DependSuffix): animated_entity.cpp
	@$(CompilerName) $(CmpOptions) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/animated_entity$(ObjectSuffix) -MF$(IntermediateDirectory)/animated_entity$(DependSuffix) -MM "/home/professor/svn-terrarium/workspace/project_terrarium/animated_entity.cpp"

$(IntermediateDirectory)/animated_entity$(PreprocessSuffix): animated_entity.cpp
	@$(CompilerName) $(CmpOptions) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/animated_entity$(PreprocessSuffix) "/home/professor/svn-terrarium/workspace/project_terrarium/animated_entity.cpp"


-include $(IntermediateDirectory)/*$(DependSuffix)
##
## Clean
##
clean:
	$(RM) $(IntermediateDirectory)/sdl$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/sdl$(DependSuffix)
	$(RM) $(IntermediateDirectory)/sdl$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/main$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/main$(DependSuffix)
	$(RM) $(IntermediateDirectory)/main$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/entity$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/entity$(DependSuffix)
	$(RM) $(IntermediateDirectory)/entity$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/actor$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/actor$(DependSuffix)
	$(RM) $(IntermediateDirectory)/actor$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/util$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/util$(DependSuffix)
	$(RM) $(IntermediateDirectory)/util$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/block$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/block$(DependSuffix)
	$(RM) $(IntermediateDirectory)/block$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/animated_entity$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/animated_entity$(DependSuffix)
	$(RM) $(IntermediateDirectory)/animated_entity$(PreprocessSuffix)
	$(RM) $(OutputFile)
	$(RM) "/home/professor/svn-terrarium/workspace/.build-debug_linux/project_terrarium"


