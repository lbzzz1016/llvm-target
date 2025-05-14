//===-- C28xAsmParser.cpp - Parse C28x assembly to MCInst instructions --===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#include "C28xGenAsmMatcher.inc"
#include "MCTargetDesc/C28xMCTargetDesc.h"
#include "TargetInfo/C28xTargetInfo.h"
#include "llvm/ADT/APInt.h"
#include "llvm/ADT/STLExtras.h"
#include "llvm/ADT/SmallVector.h"
#include "llvm/ADT/StringRef.h"
#include "llvm/ADT/StringSwitch.h"
#include "llvm/IR/FMF.h"
#include "llvm/IR/Instructions.h"
#include "llvm/MC/MCAsmMacro.h"
#include "llvm/MC/MCContext.h"
#include "llvm/MC/MCExpr.h"
#include "llvm/MC/MCInst.h"
#include "llvm/MC/MCInstBuilder.h"
#include "llvm/MC/MCInstrInfo.h"
#include "llvm/MC/MCParser/MCAsmLexer.h"
#include "llvm/MC/MCParser/MCAsmParserExtension.h"
#include "llvm/MC/MCParser/MCParsedAsmOperand.h"
#include "llvm/MC/MCParser/MCTargetAsmParser.h"
#include "llvm/MC/MCRegister.h"
#include "llvm/MC/MCRegisterInfo.h"
#include "llvm/MC/MCStreamer.h"
#include "llvm/MC/MCSubtargetInfo.h"
#include "llvm/MC/TargetRegistry.h"
#include "llvm/Support/Casting.h"
#include "llvm/Support/FormatVariadic.h"
#include "llvm/Support/SMLoc.h"
#include "llvm/Support/raw_ostream.h"
#include <cassert>
#include <cstddef>
#include <string>


using namespace llvm;

namespace {
struct C28xOperand;

class C28xAsmParser : public MCTargetAsmParser {
  SMLoc getLoc() const { return getParser().getTok().getLoc(); }

  bool MatchAndEmitInstruction(SMLoc IDLoc, unsigned &Opcode,
                               OperandVector &Operands, MCStreamer &Out,
                               uint64_t &ErrorInfo,
                               bool MatchingInlineAsm) override;

  bool parseRegister(MCRegister &Reg, SMLoc &StartLoc, SMLoc &EndLoc) override;
  ParseStatus tryParseRegister(MCRegister &Reg, SMLoc &StartLoc,
                               SMLoc &EndLoc) override;

  bool ParseInstruction(ParseInstructionInfo &Info, StringRef Name,
                        SMLoc NameLoc, OperandVector &Operands) override;
  
  void consumeTokens(unsigned Times);
  size_t peekTokensIncludingCurrent(SmallVectorImpl<AsmToken> &PeekTokens, bool ShouldSkipSpace);
  bool ParseDirective(AsmToken DirectiveID) override;
  ParseStatus parseLoc16(OperandVector &Operands);
  ParseStatus parseLoc32(OperandVector &Operands);

  bool isLexAux(StringRef &AuxReg, AsmToken Tok) {
    StringRef Reg = Tok.getString();
    if (Reg.size() == 4 && Reg.starts_with("XAR") && Reg.back() >= '0' &&
        Reg.back() < '8') {
      AuxReg = Reg;
      return true;
    }
    return false;
  }

  bool isLexSP(AsmToken Tok) {
    if (Tok.getKind() == AsmToken::Identifier &&
        Tok.getString().compare("SP") == 0) {
      return true;
    }
    return false;
  }

  // Auto-generated instruction matching functions
#define GET_ASSEMBLER_HEADER
#include "C28xGenAsmMatcher.inc"

  OperandMatchResultTy parseImmediate(OperandVector &Operands);
  OperandMatchResultTy parseRegister(OperandVector &Operands);
  OperandMatchResultTy parseSymbol(OperandVector &Operands);

  bool parseOperand(OperandVector &Operands, StringRef Mnemonic);

public:
  enum C28xMatchResultTy {
    Match_Dummy = FIRST_TARGET_MATCH_RESULT_TY,
#define GET_OPERAND_DIAGNOSTIC_TYPES
#include "C28xGenAsmMatcher.inc"
#undef GET_OPERAND_DIAGNOSTIC_TYPES
  };

  C28xAsmParser(const MCSubtargetInfo &STI, MCAsmParser &Parser,
                const MCInstrInfo &MII, const MCTargetOptions &Options)
      : MCTargetAsmParser(Options, STI, MII) {
    //  this->Parser = &Parser;
    setAvailableFeatures(ComputeAvailableFeatures(STI.getFeatureBits()));
  }
};

/// C28xOperand - Instances of this class represent a parsed machine
/// instruction
struct C28xOperand : public MCParsedAsmOperand {

  enum class KindTy {
    Token,
    Register,
    Immediate,
    RegPostInc,
    RegPreDec,
    RegReg,
    RegImm,
  } Kind;

  struct RegOp {
    unsigned RegNum;
  };

  struct ImmOp {
    const MCExpr *Val;
  };

  struct RegPostIncOp {
    unsigned RegNum;
  };

  struct RegPreDecOp {
    unsigned RegNum;
  };

  struct RegRegOp {
    unsigned RegNum1;
    unsigned RegNum2;
  };

  struct RegImmOp {
    unsigned RegNum;
    unsigned Imm;
  };

  std::string TokStor;
  SMLoc StartLoc, EndLoc;
  union {
    StringRef Tok;
    RegOp Reg;
    ImmOp Imm;
    struct RegPostIncOp RegPostInc;
    struct RegPreDecOp RegPreDec;
    struct RegRegOp RegReg;
    struct RegImmOp RegImm;
  };

  C28xOperand(KindTy K) : MCParsedAsmOperand(), Kind(K) {}

public:
  C28xOperand(const C28xOperand &O) : MCParsedAsmOperand() {
    Kind = O.Kind;
    StartLoc = O.StartLoc;
    EndLoc = O.EndLoc;
    switch (Kind) {
    case KindTy::Register:
      Reg = O.Reg;
      break;
    case KindTy::Immediate:
      Imm = O.Imm;
      break;
    case KindTy::Token:
      Tok = O.Tok;
      break;
    }
  }

  bool isToken() const override { return Kind == KindTy::Token; }
  bool isReg() const override { return Kind == KindTy::Register; }
  bool isImm() const override { return Kind == KindTy::Immediate; }
  bool isMem() const override { return false; }
  bool isLoc32() const {
    // fix: exact logic need to be implemented
    return Kind == KindTy::RegPostInc || Kind == KindTy::RegPreDec || Kind == KindTy::RegReg || Kind == KindTy::RegImm;
  }
  bool isLoc16() const { return Tok.compare("*a") == 0; }

  bool isConstantImm() const {
    return isImm() && dyn_cast<MCConstantExpr>(getImm());
  }

  int64_t getConstantImm() const {
    const MCExpr *Val = getImm();
    return static_cast<const MCConstantExpr *>(Val)->getValue();
  }

  bool isSImm12() const {
    return (isConstantImm() && isInt<12>(getConstantImm()));
  }

  /// getStartLoc - Gets location of the first token of this operand
  SMLoc getStartLoc() const override { return StartLoc; }
  /// getEndLoc - Gets location of the last token of this operand
  SMLoc getEndLoc() const override { return EndLoc; }

  MCRegister getReg() const override {
    assert(Kind == KindTy::Register && "Invalid type access!");
    //  MCRegister res(1) ;
    //  return res;
    return Reg.RegNum;
  }

  const MCExpr *getImm() const {
    assert(Kind == KindTy::Immediate && "Invalid type access!");
    return Imm.Val;
  }

  StringRef getToken() const {
    assert(Kind == KindTy::Token && "Invalid type access!");
    return Tok;
  }

  RegPostIncOp getRegPostInc() const {
    assert(Kind == KindTy::RegPostInc && "Invalid type access!");
    return RegPostInc;
  }

  RegPreDecOp getRegPreDec() const {
    assert(Kind == KindTy::RegPreDec && "Invalid type access!");
    return RegPreDec;
  }

  RegRegOp getRegReg() const {
    assert(Kind == KindTy::RegReg && "Invalid type access!");
    return RegReg;
  }

  RegImmOp getRegImm() const {
    assert(Kind == KindTy::RegImm && "Invalid type access!");
    return RegImm;
  }
  
  void print(raw_ostream &OS) const override {
    switch (Kind) {
    case KindTy::Immediate:
      OS << *getImm();
      break;
    case KindTy::Register:
      OS << "<register x";
      OS << getReg() << ">";
      break;
    case KindTy::Token:
      OS << "'" << getToken() << "'";
      break;
    case KindTy::RegPostInc:
      OS << "<regpostinc x";
      OS << getRegPostInc().RegNum << ">";
      break;
    case KindTy::RegPreDec:
      OS << "<regpredec x";
      OS << getRegPreDec().RegNum << ">";
      break;
    case KindTy::RegReg:
      OS << "<regreg x";
      OS << getRegReg().RegNum1 << ", x";
      OS << getRegReg().RegNum2 << ">";
      break;
    case KindTy::RegImm:
      OS << "<regimm x";
      OS << getRegImm().RegNum << ", ";
      OS << getRegImm().Imm << ">";
      break;
    }
  }

  static std::unique_ptr<C28xOperand> createToken(StringRef Str, SMLoc S) {
    auto Op = std::make_unique<C28xOperand>(KindTy::Token);
    Op->TokStor = Str.str();
    Op->Tok = Op->TokStor;
    Op->StartLoc = S;
    Op->EndLoc = S;
    return Op;
  }

  static std::unique_ptr<C28xOperand> createReg(unsigned RegNo, SMLoc S,
                                                SMLoc E) {
    auto Op = std::make_unique<C28xOperand>(KindTy::Register);
    Op->Reg.RegNum = RegNo;
    Op->StartLoc = S;
    Op->EndLoc = E;
    return Op;
  }

  static std::unique_ptr<C28xOperand> createImm(const MCExpr *Val, SMLoc S,
                                                SMLoc E) {
    auto Op = std::make_unique<C28xOperand>(KindTy::Immediate);
    Op->Imm.Val = Val;
    Op->StartLoc = S;
    Op->EndLoc = E;
    return Op;
  }

  static std::unique_ptr<C28xOperand> createRegPostInc(unsigned RegNo, SMLoc S,
    SMLoc E) {
    auto Op = std::make_unique<C28xOperand>(KindTy::RegPostInc);
    Op->RegPostInc.RegNum = RegNo;
    Op->StartLoc = S;
    Op->EndLoc = E;
    return Op;
  }

  static std::unique_ptr<C28xOperand> createRegPreDec(unsigned RegNo, SMLoc S,
    SMLoc E) {
    auto Op = std::make_unique<C28xOperand>(KindTy::RegPreDec);
    Op->RegPreDec.RegNum = RegNo;
    Op->StartLoc = S;
    Op->EndLoc = E;
    return Op;
  }

  static std::unique_ptr<C28xOperand> createRegReg(unsigned RegNo1, unsigned RegNo2, SMLoc S,
    SMLoc E) {
    auto Op = std::make_unique<C28xOperand>(KindTy::RegReg);
    Op->RegReg.RegNum1 = RegNo1;
    Op->RegReg.RegNum2 = RegNo2;
    Op->StartLoc = S;
    Op->EndLoc = E;
    return Op;
  }

  static std::unique_ptr<C28xOperand> createRegImm(unsigned RegNo1, int Imm, SMLoc S,
    SMLoc E) {
    auto Op = std::make_unique<C28xOperand>(KindTy::RegImm);
    Op->RegImm.RegNum = RegNo1;
    Op->RegImm.Imm = Imm ;
    Op->StartLoc = S;
    Op->EndLoc = E;
    return Op;
  }

  void addExpr(MCInst &Inst, const MCExpr *Expr) const {
    assert(Expr && "Expr shouldn't be null!");
    if (auto *CE = dyn_cast<MCConstantExpr>(Expr))
      Inst.addOperand(MCOperand::createImm(CE->getValue()));
    else
      Inst.addOperand(MCOperand::createExpr(Expr));
  }

  // Used by the TableGen Code
  void addRegOperands(MCInst &Inst, unsigned N) const {
    assert(N == 1 && "Invalid number of operands!");
    Inst.addOperand(MCOperand::createReg(getReg()));
  }

  void addImmOperands(MCInst &Inst, unsigned N) const {
    assert(N == 1 && "Invalid number of operands!");
    addExpr(Inst, getImm());
  }

  void addLoc32Operands(MCInst &Inst, unsigned N) const {
    assert(N == 1 && "Invalid number of operands!");
    MCInst *Val = new MCInst();
    switch (Kind) {
      default: break;
    case KindTy::RegPostInc:
      Val->setOpcode(C28x::AMPostInc);
      Val->addOperand(MCOperand::createReg(RegPostInc.RegNum));
      Inst.addOperand(MCOperand::createInst(Val));
      break;
    case KindTy::RegPreDec:
      Val->setOpcode(C28x::AMPreDec);
      Val->addOperand(MCOperand::createReg(RegPreDec.RegNum));
      Inst.addOperand(MCOperand::createInst(Val));
      break;
    case KindTy::RegReg:
      Val->setOpcode(C28x::AMRegReg);
      Val->addOperand(MCOperand::createReg(RegReg.RegNum1));
      Val->addOperand(MCOperand::createReg(RegReg.RegNum2));
      Inst.addOperand(MCOperand::createInst(Val));
      break;
    case KindTy::RegImm:
      Val->setOpcode(C28x::AMRegImm);
      Val->addOperand(MCOperand::createReg(RegImm.RegNum));
      Val->addOperand(MCOperand::createImm(RegImm.Imm));
      Inst.addOperand(MCOperand::createInst(Val));
      break;
    }
    return;
  }
};
} // end anonymous namespace.

#define GET_REGISTER_MATCHER
#define GET_MATCHER_IMPLEMENTATION
#include "C28xGenAsmMatcher.inc"

bool C28xAsmParser::MatchAndEmitInstruction(SMLoc IDLoc, unsigned &Opcode,
                                            OperandVector &Operands,
                                            MCStreamer &Out,
                                            uint64_t &ErrorInfo,
                                            bool MatchingInlineAsm) {

  MCInst Inst;
  SMLoc ErrorLoc;

  switch (MatchInstructionImpl(Operands, Inst, ErrorInfo, MatchingInlineAsm)) {
  default:
    break;
  case Match_Success:
    Inst.setLoc(IDLoc);
    Out.emitInstruction(Inst, getSTI());
    return false;
  case Match_MissingFeature:
    return Error(IDLoc, "instruction use requires an option to be enabled");
  case Match_MnemonicFail:
    return Error(IDLoc, "unrecognized instruction mnemonic");
  case Match_InvalidOperand:
    ErrorLoc = IDLoc;
    if (ErrorInfo != ~0U) {
      if (ErrorInfo >= Operands.size())
        return Error(ErrorLoc, "too few operands for instruction");

      ErrorLoc = ((C28xOperand &)*Operands[ErrorInfo]).getStartLoc();
      if (ErrorLoc == SMLoc())
        ErrorLoc = IDLoc;
    }
    return Error(ErrorLoc, "invalid operand for instruction");
  case Match_InvalidSImm12:
    SMLoc ErrorLoc = ((C28xOperand &)*Operands[ErrorInfo]).getStartLoc();
    return Error(ErrorLoc,
                 "immediate must be an integer in the range [-2048, 2047]");
  }

  llvm_unreachable("Unknown match type detected!");
}

ParseStatus C28xAsmParser::tryParseRegister(MCRegister &Reg, SMLoc &StartLoc,
                                            SMLoc &EndLoc) {
  const AsmToken &Tok = getParser().getTok();
  StartLoc = Tok.getLoc();
  EndLoc = Tok.getEndLoc();
  StringRef Name = getLexer().getTok().getIdentifier();

  return ParseStatus::Success;
}

bool C28xAsmParser::parseRegister(MCRegister &Reg, SMLoc &StartLoc,
                                  SMLoc &EndLoc) {

  return false;
}

OperandMatchResultTy C28xAsmParser::parseRegister(OperandVector &Operands) {
  SMLoc S = getLoc();
  SMLoc E = SMLoc::getFromPointer(S.getPointer() - 1);
  switch (getLexer().getKind()) {
  default:
    return MatchOperand_NoMatch;
  case AsmToken::Identifier:
    StringRef Name = getLexer().getTok().getIdentifier();
    // Name = "ACC, XT"
    unsigned RegNo = MatchRegisterName(Name);
    if (RegNo == 0) {
      RegNo = MatchRegisterAltName(Name);
      if (RegNo == 0)
        return MatchOperand_NoMatch;
    }
    getLexer().Lex();
    Operands.push_back(C28xOperand::createReg(RegNo, S, E));
  }
  return MatchOperand_Success;
}

OperandMatchResultTy C28xAsmParser::parseImmediate(OperandVector &Operands) {

  switch (getLexer().getKind()) {
  default:
    return MatchOperand_NoMatch;
  case AsmToken::LParen:
  case AsmToken::Minus:
  case AsmToken::Plus:
  case AsmToken::Integer:
  case AsmToken::String:
    break;
  }
  SMLoc S = getLoc();

  int64_t ImmVal = getTok().getIntVal();
  getLexer().Lex(); 
  const MCExpr *ImmExpr = MCConstantExpr::create(ImmVal, getContext());

  SMLoc E = SMLoc::getFromPointer(S.getPointer() - 1);
  Operands.push_back(C28xOperand::createImm(ImmExpr, S, E));
  return MatchOperand_Success;
}

OperandMatchResultTy C28xAsmParser::parseSymbol(OperandVector &Operands) {
  switch (getLexer().getKind()) {
  default:
    return MatchOperand_NoMatch;
  case AsmToken::LessLess:
  case AsmToken::Identifier:
  case AsmToken::Star:
    // case AsmToken::Hash:
    break;
  }
  SMLoc S = getLoc();

  StringRef Val = getLexer().getTok().getString();
  Operands.push_back(C28xOperand::createToken(Val, S));
  getLexer().Lex();
  return MatchOperand_Success;
}

/// Looks at a token type and creates the relevant operand
/// from this information, adding to Operands.
/// If operand was parsed, returns false, else true.
bool C28xAsmParser::parseOperand(OperandVector &Operands, StringRef Mnemonic) {
  // C28xOperand menmonic = (C28xOperand &)*Operands.front();
  ParseStatus ResTy = MatchOperandParserImpl(Operands, Mnemonic);
  if (ResTy.isSuccess())
    return false;

  if (ResTy.isFailure()) {
    return true;
  }

  switch (getLexer().getKind()) {
  default:
    return true;
  case AsmToken::Identifier:
    if (parseRegister(Operands) == MatchOperand_Success)
      return false;
    return true;
  case AsmToken::Integer:
    if (parseImmediate(Operands) == MatchOperand_Success)
      return false;
    return true;
  case AsmToken::LessLess:
  case AsmToken::Plus:
  case AsmToken::Minus:
  case AsmToken::GreaterGreater: {
    SMLoc S = getLoc();
    StringRef Val = getLexer().getTok().getString();
    Operands.push_back(C28xOperand::createToken(Val, S));
    getLexer().Lex();
    return false;
  }
  case AsmToken::Hash:
    getLexer().Lex();
    if (parseImmediate(Operands) == MatchOperand_Success)
      return false;
    return true;


  }

  // Finally we have exhausted all options and must declare defeat.
  //  Error(getLoc(), "unknown operand");
  return true;
}

bool C28xAsmParser::ParseInstruction(ParseInstructionInfo &Info, StringRef Name,
                                     SMLoc NameLoc, OperandVector &Operands) {

  // First operand is token for instruction
  Operands.push_back(C28xOperand::createToken(Name, NameLoc));

  SMLoc Pre = getLoc();

  while (getLexer().isNot(AsmToken::EndOfStatement)) {
    if (getLexer().is(AsmToken::Comma)) {
      // Consume comma token
      getLexer().Lex();
      continue;
    }
    if (parseOperand(Operands, Name))
      return true;
    if (Pre == getLoc()) {
      break;
    }
  }

  if (getLexer().isNot(AsmToken::EndOfStatement)) {
    SMLoc Loc = getLexer().getLoc();
    getParser().eatToEndOfStatement();
    return Error(Loc, "unexpected token");
  }


  getParser().Lex(); // Consume the EndOfStatement.
  return false;
}

void C28xAsmParser::consumeTokens(unsigned Times) {
  for (int I = 0; I < Times; I++) {
    getLexer().Lex();
  }
}

size_t C28xAsmParser::peekTokensIncludingCurrent(SmallVectorImpl<AsmToken> &PeekTokens, bool ShouldSkipSpace = true) {
  PeekTokens.push_back(getLexer().getTok());
  AsmToken Buf[6];
  size_t PeekNum = getLexer().peekTokens(Buf);
  for (int I = 0; I < PeekNum; I++) {
    PeekTokens.push_back(Buf[I]);
  }
  return PeekTokens.size();
}

bool C28xAsmParser::ParseDirective(AsmToken DirectiveID) {
  printf("ParseDirective execute\r\n");
  return true;
}

ParseStatus C28xAsmParser::parseLoc16(OperandVector &Operands) { 
  return ParseStatus::NoMatch;
}

ParseStatus C28xAsmParser::parseLoc32(OperandVector &Operands) {
  SmallVector<AsmToken, 8> PeekTokens;
  size_t TokSize = peekTokensIncludingCurrent(PeekTokens);

  if (PeekTokens[0].is(AsmToken::Star)) {
    SMLoc S = getLoc();
    if (PeekTokens[1].is(AsmToken::Minus)) {
      if (PeekTokens[2].is(AsmToken::Minus)) {
        StringRef Reg;
        if (isLexSP(PeekTokens[3])) {
          Operands.push_back(C28xOperand::createRegPreDec(C28x::SP, S, S));
          consumeTokens(4);
          return ParseStatus::Success;
        } else if (isLexAux(Reg, PeekTokens[3])) {
          MCRegister MCReg = MatchRegisterName(Reg);
          Operands.push_back(C28xOperand::createRegPreDec(MCReg.id(), S, S));
          consumeTokens(4);
          return ParseStatus::Success;
        }
      } else if (isLexSP(PeekTokens[2]) && PeekTokens[3].is(AsmToken::LBrac)) {
        AsmToken Int = PeekTokens[4];
        if (Int.getIntVal() < 64 && Int.getIntVal() >= 0) {
          if (PeekTokens[5].is(AsmToken::RBrac)) {
            Operands.push_back(C28xOperand::createRegImm(C28x::SP, Int.getIntVal(), S, S));
            consumeTokens(6);
            return ParseStatus::Success;
          }
        }
      }
    }
    StringRef Reg = "SP";
    if ((isLexSP(PeekTokens[1]) || isLexAux(Reg, PeekTokens[1])) && PeekTokens[2].is(AsmToken::Plus) && PeekTokens[3].is(AsmToken::Plus)) {
      MCRegister MCReg = MatchRegisterName(Reg);
      Operands.push_back(C28xOperand::createRegPostInc(MCReg, S, S));
      consumeTokens(4);
      return ParseStatus::Success;
    }

    if (PeekTokens[1].is(AsmToken::Plus) && isLexAux(Reg, PeekTokens[2]) && PeekTokens[3].is(AsmToken::LBrac)) {
      AsmToken AR = PeekTokens[4];
      if (AR.getString().compare("AR0") == 0 || AR.getString().compare("AR1") == 0) {
        if (PeekTokens[5].is(AsmToken::RBrac)) {
          Operands.push_back(C28xOperand::createRegReg(MatchRegisterName(Reg), MatchRegisterName(AR.getString()), S, S));
          consumeTokens(6);
          return ParseStatus::Success;
        }
      }else if (AR.getIntVal() >= 0  && AR.getIntVal() < 8) {
        if (PeekTokens[5].is(AsmToken::RBrac)) {
          Operands.push_back(C28xOperand::createRegImm(MatchRegisterName(Reg), AR.getIntVal(), S, S));
          consumeTokens(6);
          return ParseStatus::Success;
        }
      }
    }

  }

  if (parseOptionalToken(AsmToken::At)) {
    return ParseStatus::Success;
  }

  return ParseStatus::NoMatch;
}

extern "C" LLVM_EXTERNAL_VISIBILITY void LLVMInitializeC28xAsmParser() {
  RegisterMCAsmParser<C28xAsmParser> X(getTheC28xTarget());
}